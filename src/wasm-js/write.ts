import {
  mem32,
  mem16,
  wasmExports,
  wasmConstAddrs,
  Tag,
  Header,
  WORD,
  SIZE_SHIFT
} from './shared';
import { appTypes } from './generated';

const writeString = (s: string) => (idx32: number): number => {
  const offset = idx32 << 1;
  const len = s.length + (s.length % 2); // for odd length, write an extra word (gets coerced to 0)
  for (let i = 0; i < len; i++) {
    mem16[offset + i] = s.charCodeAt(i);
  }
  return len >> 1;
};

function encodeHeader({ tag, size }: Header): number {
  return tag | (size << SIZE_SHIFT);
}

/**
 * Write a value to the Wasm memory and return the address where it was written
 * @param mem32 WebAssembly memory
 * @param maxIndex  Current index into the memory
 * @param value  JavaScript Elm value to send to Wasm
 * @returns address written and next index to write
 */
export function writeValue(
  maxIndex: number,
  value: any
): { addr: number; maxIndex: number } {
  let tag: Tag;

  switch (typeof value) {
    case 'number':
      tag = Number.isInteger(value) ? Tag.Int : Tag.Float; // UNSAFE!
      break;
    case 'string':
      tag = Tag.String;
      break;
    case 'boolean':
      return {
        addr: value ? wasmConstAddrs.True : wasmConstAddrs.False,
        maxIndex
      };
    case 'function':
      tag = Tag.Closure;
      break;
    case 'object': {
      if (value instanceof String) {
        tag = Tag.Char;
        break;
      }
      switch (value.$) {
        case undefined:
          tag = Tag.Record;
          break;
        case '[]':
          return { addr: wasmConstAddrs.Nil, maxIndex };
        case '::':
          tag = Tag.List;
          break;
        case '#0':
          return { addr: wasmConstAddrs.Unit, maxIndex };
        case '#2':
          tag = Tag.Tuple2;
          break;
        case '#3':
          tag = Tag.Tuple3;
          break;
        case '#0':
        default:
          tag = Tag.Custom;
          break;
      }
    }
  }

  let body: number[] = [];
  let children: any[] = [];
  let writer: (addr: number) => number;
  switch (tag) {
    case Tag.Int: {
      body[0] = value | 0;
      break;
    }
    case Tag.Float: {
      writer = addr => {
        wasmExports.writeFloat(addr, value);
        return 2; // words written
      };
      break;
    }
    case Tag.Char:
    case Tag.String: {
      writer = writeString(value);
      break;
    }
    case Tag.Tuple2:
    case Tag.List: {
      children = [value.a, value.b];
      break;
    }
    case Tag.Tuple3: {
      children = [value.a, value.b, value.c];
      break;
    }
    case Tag.Custom: {
      const jsCtor: string = value.$;
      body[0] = appTypes.ctors[jsCtor];
      Object.keys(value).forEach(k => {
        if (k !== '$') children.push(value[k]);
      });
      break;
    }
    case Tag.Record: {
      const keys = Object.keys(value);
      keys.sort();
      body[0] = appTypes.fieldGroups[keys.join('$')];
      children = keys.map(k => value[k]);
      break;
    }
    case Tag.Closure: {
      break;
    }
  }

  let index = maxIndex;

  const childAddrs: number[] = [];
  children.forEach(child => {
    const update = writeValue(index, child);
    childAddrs.push(update.addr);
    index = update.maxIndex;
  });

  const currentAddr = index * WORD;
  if (writer) {
    const size = 1 + writer((index + 1) * WORD);
    mem32[index] = encodeHeader({ tag, size });
    index += size;
  } else {
    const size = body.length + children.length;
    mem32[index++] = encodeHeader({ tag, size });
    body.forEach(word => {
      mem32[index++] = word;
    });
    childAddrs.forEach(addr => {
      mem32[index++] = addr;
    });
  }

  return {
    addr: currentAddr,
    maxIndex: index
  };
}
