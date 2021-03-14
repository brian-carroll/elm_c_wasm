const prettier = require('prettier');

declare const Module: { HEAPU32: Uint32Array;  HEAPU16: Uint16Array; };

interface Patch {
  ctor: number;
  number: number;
  values: Uint32Array;
  next: number;
}

let mem32: Uint32Array = Module.HEAPU32;
let mem16: Uint16Array = Module.HEAPU16;
let rootDomNode: Node | null = document.getElementById('main'); // TODO
const textDecoder = new TextDecoder('utf-16le');

function hex(n: number): string {
  return '0x' + n.toString(16);
}

function hexArray(mem: Uint32Array): string {
  let strings: string[] = [];
  mem.forEach(x => {
    strings.push(hex(x));
  });
  return strings.join(' ');
}

function htmlString(node: Node | null): string {
  let html = '';
  if (!node) {
    html = `${node}`;
  } else if (node instanceof window.HTMLElement) {
    html = node.outerHTML;
  } else {
    html = node.textContent || node.toString();
  }

  return prettier.format(html, { parser: 'html' });
}

const vdom_ctor_names: string[] = [
  '',
  'VDOM_NODE',       // values: tag, facts, children
  'VDOM_NODE_KEYED',
  'VDOM_NODE_NS',
  'VDOM_NODE_NS_KEYED',
  'VDOM_NODE_TEXT',
  'VDOM_NODE_TAGGER',
  'VDOM_NODE_THUNK',
  'VDOM_FACT_EVENT',
  'VDOM_FACT_STYLE',
  'VDOM_FACT_PROP',
  'VDOM_FACT_ATTR',
  'VDOM_FACT_ATTR_NS',
  'VDOM_PATCH_PUSH', // number: child_index
  'VDOM_PATCH_POP', // (no data)
  'VDOM_PATCH_LINK', // values: vdom_patch
  'VDOM_PATCH_NO_OP', // (no data)
  'VDOM_PATCH_END', // (no data)
  'VDOM_PATCH_REDRAW', // values: vdom_node
  'VDOM_PATCH_SET_EVENT', // number: 2, values: key, value
  'VDOM_PATCH_SET_STYLE', // number: 2, values: key, value
  'VDOM_PATCH_SET_PROP', // number: 2, values: key, value
  'VDOM_PATCH_SET_ATTR', // number: 2, values: key, value
  'VDOM_PATCH_SET_ATTR_NS', // number: 3, values: namespace, key, value
  'VDOM_PATCH_REMOVE_EVENT', // number: 1, values: key
  'VDOM_PATCH_REMOVE_STYLE', // number: 1, values: key
  'VDOM_PATCH_REMOVE_PROP', // number: 1, values: key
  'VDOM_PATCH_REMOVE_ATTR', // number: 1, values: key
  'VDOM_PATCH_REMOVE_ATTR_NS', // number: 2, values: namespace, key
  'VDOM_PATCH_TEXT', // values: replacement_string_ptr
  // 'VDOM_PATCH_THUNK', // values: sub_patches[]
  'VDOM_PATCH_TAGGER', // values: tagger, eventNode??
  'VDOM_PATCH_REMOVE_LAST', // number: num_to_remove
  'VDOM_PATCH_APPEND', // number: nKids, values: kids
  'VDOM_PATCH_REMOVE', // (keyed only)
  'VDOM_PATCH_REORDER' // (keyed only) patches, inserts, endInserts
  // 'VDOM_PATCH_CUSTOM' // ? no idea what this is
];

let vdom_ctor = 1;
const VDOM_NODE = vdom_ctor++;
const VDOM_NODE_KEYED = vdom_ctor++;
const VDOM_NODE_NS = vdom_ctor++;
const VDOM_NODE_NS_KEYED = vdom_ctor++;
const VDOM_NODE_TEXT = vdom_ctor++;
const VDOM_NODE_TAGGER = vdom_ctor++;
const VDOM_NODE_THUNK = vdom_ctor++;
const VDOM_FACT_EVENT = vdom_ctor++;
const VDOM_FACT_STYLE = vdom_ctor++;
const VDOM_FACT_PROP = vdom_ctor++;
const VDOM_FACT_ATTR = vdom_ctor++;
const VDOM_FACT_ATTR_NS = vdom_ctor++;
const VDOM_PATCH_PUSH = vdom_ctor++;
const VDOM_PATCH_POP = vdom_ctor++;
const VDOM_PATCH_LINK = vdom_ctor++;
const VDOM_PATCH_NO_OP = vdom_ctor++;
const VDOM_PATCH_END = vdom_ctor++;
const VDOM_PATCH_REDRAW = vdom_ctor++;
const VDOM_PATCH_SET_EVENT = vdom_ctor++;
const VDOM_PATCH_SET_STYLE = vdom_ctor++;
const VDOM_PATCH_SET_PROP = vdom_ctor++;
const VDOM_PATCH_SET_ATTR = vdom_ctor++;
const VDOM_PATCH_SET_ATTR_NS = vdom_ctor++;
const VDOM_PATCH_REMOVE_EVENT = vdom_ctor++;
const VDOM_PATCH_REMOVE_STYLE = vdom_ctor++;
const VDOM_PATCH_REMOVE_PROP = vdom_ctor++;
const VDOM_PATCH_REMOVE_ATTR = vdom_ctor++;
const VDOM_PATCH_REMOVE_ATTR_NS = vdom_ctor++;
const VDOM_PATCH_TEXT = vdom_ctor++;
// const VDOM_PATCH_THUNK = vdom_patch_ctor++;
const VDOM_PATCH_TAGGER = vdom_ctor++;
const VDOM_PATCH_REMOVE_LAST = vdom_ctor++;
const VDOM_PATCH_APPEND = vdom_ctor++;
const VDOM_PATCH_REMOVE = vdom_ctor++;
const VDOM_PATCH_REORDER = vdom_ctor++;
// const VDOM_PATCH_CUSTOM = vdom_patch_ctor++;

var VirtualDom_init = (virtualNode: number) => (flagDecoder: number) => (debugMetadata: any) => (args: {node: HTMLElement}) => {
  rootDomNode = args['node'];
  mem32 = Module.HEAPU32;
  mem16 = Module.HEAPU16;
  redraw(rootDomNode, virtualNode);
}

var VirtualDom_applyPatches = function (patchesStartAddr: number) {
  if (!rootDomNode) throw new Error('VirtualDom cannot render. Root DOM node is '+ rootDomNode);
  mem32 = Module.HEAPU32; // TODO: delete
  mem16 = Module.HEAPU16; // TODO: delete
  let addr = patchesStartAddr;
  do {
    const patch = readPatch(addr);
    addr = applyPatch(rootDomNode, patch);
  } while (addr);
  console.log('rendered DOM:\n' + htmlString(rootDomNode));
};

function readPatch(addr: number): Patch {
  const index = addr >> 2;
  const header = mem32[index];
  const ctor = header & 0xff;
  const number = header >> 8;
  let n_values;
  switch (ctor) {
    case VDOM_PATCH_PUSH:
    case VDOM_PATCH_REMOVE_LAST:
      n_values = 0;
      break;
    default:
      n_values = number;
      break;
  }
  const values = mem32.slice(index + 1, index + 1 + n_values);
  const next = addr + ((1 + n_values) << 2);
  const patch = { ctor, number, values, next };
  console.log('readPatch', {
    addr: hex(addr),
    header: hex(header),
    ctor: vdom_ctor_names[ctor],
    number,
    values: hexArray(values),
    next: hex(next),
    n_values,
  });
  return patch;
}

function applyPatch(node: Node, patch: Patch): number {
  switch (patch.ctor) {
    case VDOM_PATCH_PUSH: {
      const elem = node as Element;
      const nextPatch = readPatch(patch.next);
      return applyPatch(elem.children[patch.number], nextPatch);
    }
    case VDOM_PATCH_REDRAW: {
      redraw(node, patch.values[0]);
      return patch.next;
    }

    case VDOM_PATCH_NO_OP:
      return patch.next;

    case VDOM_PATCH_END:
      return 0;

    case VDOM_PATCH_POP:
      return patch.next;

    case VDOM_PATCH_LINK:
      return patch.values[0];

    case VDOM_PATCH_SET_EVENT:
    case VDOM_PATCH_SET_STYLE:
    case VDOM_PATCH_SET_PROP:
    case VDOM_PATCH_SET_ATTR:
    case VDOM_PATCH_SET_ATTR_NS:
    case VDOM_PATCH_REMOVE_EVENT:
    case VDOM_PATCH_REMOVE_STYLE:
    case VDOM_PATCH_REMOVE_PROP:
    case VDOM_PATCH_REMOVE_ATTR:
    case VDOM_PATCH_REMOVE_ATTR_NS:
    case VDOM_PATCH_TEXT:
    case VDOM_PATCH_TAGGER:
    case VDOM_PATCH_REMOVE_LAST:
    case VDOM_PATCH_APPEND:
    case VDOM_PATCH_REMOVE:
    case VDOM_PATCH_REORDER:
      throw new Error('TODO: patch type ' + vdom_ctor_names[patch.ctor]);
    default:
      throw new Error(`Unknown virtual DOM patch type ${patch.ctor}`);
  }
}

function redraw(dom: Node, vdom_node_addr: number) {
  const newNode = createNode(vdom_node_addr);
  const parent = dom.parentElement;
  parent?.replaceChild(newNode, dom);
  if (dom === rootDomNode) {
    rootDomNode = newNode;
  }
}

function readString(addr: number): string {
  const index = addr >> 2;
  const header = mem32[index];
  const tag = header >> 28;
  if (tag !== 3) throw new Error('Expected a String at 0x' + addr.toString(16));
  const size = header & 0x00ffffff;

  const idx16 = (index + 1) << 1;
  let len16 = (size - 1) << 1;
  if (mem16[idx16 + len16 - 1] === 0) len16--;
  const words16 = mem16.slice(idx16, idx16 + len16);
  const jsString = textDecoder.decode(words16);
  return jsString;
}

function createNode(addr: number): Node {
  const index = addr >> 2;
  const header = mem32[index];
  const ctor = header & 0xff;

  if (ctor === VDOM_NODE_TEXT) {
    const contentAddr = mem32[index + 1];
    const content = readString(contentAddr);
    console.log('createNode', {
      addr: hex(addr),
      header: hex(header),
      ctor: vdom_ctor_names[ctor],
      content,
    });  
    return document.createTextNode(content);
  }

  const n_extras = (header >> 8) & 0xff;
  const n_facts = (header >> 16) & 0xff;
  const n_children = header >> 24;
  let idx = index + 1;
  const extras = mem32.slice(idx, idx += n_extras);
  const facts = mem32.slice(idx, idx += n_facts);
  const children = mem32.slice(idx, idx += n_children);
  const extras0 = readString(extras[0]);

  console.log('createNode', {
    addr: hex(addr),
    header: hex(header),
    ctor: vdom_ctor_names[ctor],
    tag: extras0,
    extras: hexArray(extras),
    facts: hexArray(facts),
    children: hexArray(children),
  })

  switch (ctor) {
    case VDOM_NODE: {
      const node = document.createElement(extras0);
      for (let i = facts.length - 1; i >= 0; --i) {
        const fAddr = facts[i];
        applyFact(node, fAddr);
      }
      for (let i = children.length - 1; i >= 0; --i) {
        const cAddr = children[i];
        node.appendChild(createNode(cAddr));
      }
      return node;
    }
    case VDOM_NODE_KEYED:
    case VDOM_NODE_NS:
    case VDOM_NODE_NS_KEYED:
    case VDOM_NODE_TAGGER:
    case VDOM_NODE_THUNK: {
      const node = document.createElement('i');
      const text = document.createTextNode('TODO: ' + vdom_ctor_names[ctor]);
      node.style.color = 'red';
      node.appendChild(text);
      return node; 
    }
    default:
      throw new Error(`Unknown virtual DOM node type ${ctor}`);
  }
}

function applyFact(node: HTMLElement, addr: number) {
  const index = addr >> 2;
  const ctor = mem32[index] & 0xff;
  const key = readString(mem32[index + 1]);
  const valueAddr = mem32[index + 2];

  console.log('applyFact', {
    addr: hex(addr),
    ctor,
    ctorString: vdom_ctor_names[ctor],
    keyAddr: hex(mem32[index + 1]),
    key,
    valueAddr: hex(valueAddr),
    value: readString(valueAddr),
  });

  switch (ctor) {
    case VDOM_FACT_STYLE: {
      const value = readString(valueAddr);
      node.style[key as any] = value;
      break;
    }
    case VDOM_FACT_ATTR: {
      const value = readString(valueAddr);
      node.setAttribute(key, value);
      break;
    }
    case VDOM_FACT_EVENT:
    case VDOM_FACT_PROP:
    case VDOM_FACT_ATTR_NS:
      console.warn('TODO: fact type ' + vdom_ctor_names[ctor]);
      break;
    default:
      throw new Error(`Unknown virtual DOM fact type ${ctor}`);
  }

}