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

/**
 * Write a value to the Wasm memory and return the address where it was written
 *
 * Wasm structures all have the same general layout
 *   - header (containing the size in 32-bit words and a type tag)
 *   - body data
 *   - child pointers
 *
 * @param nextIndex Next available index into the memory
 * @param value  JavaScript Elm value to send to Wasm
 * @returns address written and next index to write
 */
export function writeValue(
  nextIndex: number,
  value: any
): { addr: number; nextIndex: number } {
  let tag: Tag = Tag.Unused;

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
        nextIndex
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
          return { addr: wasmConstAddrs.Nil, nextIndex };
        case '::':
          tag = Tag.List;
          break;
        case '#0':
          return { addr: wasmConstAddrs.Unit, nextIndex };
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
  let jsChildren: any[] = [];
  let writer: ((addr: number) => number) | undefined;
  switch (tag) {
    case Tag.Int: {
      body[0] = value;
      break;
    }
    case Tag.Float: {
      writer = (addr: number) => {
        wasmExports.writeFloat(addr, value);
        return 2; // words written
      };
      break;
    }
    case Tag.Char:
    case Tag.String: {
      writer = (addr: number) => writeString(value, addr);
      break;
    }
    case Tag.Tuple2:
    case Tag.List: {
      jsChildren = [value.a, value.b];
      break;
    }
    case Tag.Tuple3: {
      jsChildren = [value.a, value.b, value.c];
      break;
    }
    case Tag.Custom: {
      const jsCtor: string = value.$;
      body[0] = appTypes.ctors[jsCtor];
      Object.keys(value).forEach(k => {
        if (k !== '$') jsChildren.push(value[k]);
      });
      break;
    }
    case Tag.Record: {
      const keys = Object.keys(value);
      keys.sort();
      const fgName = keys.join('$');
      const fgAddr = appTypes.fieldGroups[fgName];
      body[0] = fgAddr;
      jsChildren = keys.map(k => value[k]);
      break;
    }
    case Tag.Closure: {
      break;
    }
  }

  const childAddrs: number[] = [];
  jsChildren.forEach(child => {
    const update = writeValue(nextIndex, child);
    childAddrs.push(update.addr);
    nextIndex = update.nextIndex;
  });

  const currentAddr = nextIndex * WORD;
  if (writer) {
    const size = 1 + writer((nextIndex + 1) * WORD);
    mem32[nextIndex] = encodeHeader({ tag, size });
    nextIndex += size;
  } else {
    const size = body.length + jsChildren.length;
    mem32[nextIndex++] = encodeHeader({ tag, size });
    body.forEach(word => {
      mem32[nextIndex++] = word;
    });
    childAddrs.forEach(addr => {
      mem32[nextIndex++] = addr;
    });
  }

  return {
    addr: currentAddr,
    nextIndex
  };
}

function encodeHeader({ tag, size }: Header): number {
  return tag | (size << SIZE_SHIFT);
}

function writeString(s: string, idx32: number): number {
  const offset = idx32 << 1;
  const len = s.length + (s.length % 2); // for odd length, write an extra word (gets coerced to 0)
  for (let i = 0; i < len; i++) {
    mem16[offset + i] = s.charCodeAt(i);
  }
  return len >> 1;
}
