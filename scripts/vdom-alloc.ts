import { assert } from 'console';

enum NodeType {
  TEXT = 'TEXT',
  NODE = 'NODE',
  KEYED_NODE = 'KEYED_NODE',
  TAGGER = 'TAGGER',
  THUNK = 'THUNK'
  // CUSTOM = 'CUSTOM', // unused in elm org libs!
}

enum FactType {
  EVENT = 'EVENT',
  STYLE = 'STYLE',
  PROP = 'PROP',
  ATTR = 'ATTR',
  ATTR_NS = 'ATTR_NS'
}

class VdomArray<I extends Index, W, O> {
  public words: W[];
  public bottom: I;

  constructor(size: I) {
    this.words = new Array<W>(size.index);
    this.bottom = size;
  }

  toString(): string {
    const lines: string[] = [];
    this.words.forEach((w, i) => {
      if (w == undefined) {
        return;
      } else if (typeof w === 'string') {
        lines.push(`${i}\t"${w}"`);
      } else {
        lines.push(`${i}\t${w}`);
      }
    });
    return lines.join('\n');
  }

  prependWords(...words: W[]): I {
    let newBottom = this.bottom.index - words.length;
    if (newBottom < 0) {
      console.warn('resizing', this.bottom);
      const oldData = this.words;
      const oldLen = oldData.length;
      this.words = new Array<W>(oldLen * 2);
      oldData.forEach((word, i) => {
        this.words[oldLen + i] = word;
      });
      newBottom += oldLen;
    }

    words.forEach((word, i) => {
      this.words[newBottom + i] = word;
    });

    this.bottom = Object.create(this.bottom);
    this.bottom.index = newBottom;
    return this.bottom;
  }

  prependObject(obj: O): I {
    const words: W[] = [];
    Object.values(obj).forEach(val => {
      if (Array.isArray(val)) {
        words.push(...val);
      } else {
        words.push(val);
      }
    });
    return this.prependWords(...words);
  }
}

class Index {
  constructor(public index: number) {}
}

class NodeIndex extends Index {
  toString() {
    return `Node ${this.index}\t\t-> ${nodes.words[this.index]}`;
  }
}
class FactIndex extends Index {
  toString() {
    return `Fact ${this.index}\t\t-> ${facts.words[this.index]}`;
  }
}
class ElmIndex extends Index {
  toString() {
    return `ElmValue ${this.index}\t-> ${elmValues.words[this.index]}`;
  }
}

class Size {
  constructor(public size: number) {}
  toString() {
    return `{size: ${this.size}}`;
  }
}

class List<T> {
  constructor(public head: T, public tail: List<T> | null = null) {}
}

const nodes = new VdomArray<NodeIndex, NodeWord, VdomNode>(new NodeIndex(100));
const facts = new VdomArray<FactIndex, FactWord, VdomFact>(new FactIndex(100));
const elmValues = new VdomArray<ElmIndex, any, any>(new ElmIndex(100));

type NodeWord = NodeType | number | ElmIndex | NodeIndex | FactIndex;
type FactWord = FactType | ElmIndex;

interface NodeText {
  type: NodeType.TEXT;
  size: Size;
  valueIndex: ElmIndex;
}

interface NodeNormal {
  type: NodeType.NODE;
  size: Size;
  tag: ElmIndex;
  nFacts: number;
  facts: FactIndex[];
  children: NodeIndex[];
}

interface NodeNS extends NodeNormal {
  namespace: ElmIndex;
}

interface NodeKeyed {
  type: NodeType.KEYED_NODE;
  size: Size;
  tag: ElmIndex;
  namespace: ElmIndex;
  nFacts: number;
  facts: FactIndex[];
  keysBegin: ElmIndex;
  children: NodeIndex[];
}

interface NodeKeyedNS extends NodeKeyed {
  namespace: ElmIndex;
}

interface NodeTagger {
  type: NodeType.TAGGER;
  size: Size;
  tagger: ElmIndex;
  nodeIndex: NodeIndex;
}

interface NodeThunk {
  type: NodeType.THUNK;
  size: Size;
  nodeIndex: NodeIndex;
  thunk: ElmIndex;
  refs: ElmIndex[];
}

type VdomNode = NodeText | NodeNormal | NodeKeyed | NodeTagger | NodeThunk;

interface FactEvent {
  type: FactType.EVENT;
  key: ElmIndex;
  handler: ElmIndex;
}

interface FactStyle {
  type: FactType.STYLE;
  key: ElmIndex;
  value: ElmIndex;
}

interface FactProp {
  type: FactType.PROP;
  key: ElmIndex;
  value: ElmIndex;
}

interface FactAttr {
  type: FactType.ATTR;
  key: ElmIndex;
  value: ElmIndex;
}

interface FactAttrNS {
  type: FactType.ATTR_NS;
  key: ElmIndex;
  value: ElmIndex;
  namespace: ElmIndex;
}

type VdomFact = FactEvent | FactStyle | FactProp | FactAttr | FactAttrNS;

const text = (content: string): NodeIndex => {
  const valueIndex = elmValues.prependWords(content);
  const obj: NodeText = {
    type: NodeType.TEXT,
    size: new Size(3),
    valueIndex
  };
  return nodes.prependObject(obj);
};

const node = (tag: string) => (
  factList: List<FactIndex> | null,
  kidList: List<NodeIndex> | null
) => {
  const facts: FactIndex[] = [];
  const children: NodeIndex[] = [];
  for (; factList != null; factList = factList.tail) {
    facts.unshift(factList.head);
  }
  for (; kidList != null; kidList = kidList.tail) {
    children.unshift(kidList.head);
  }

  const obj: NodeNormal = {
    type: NodeType.NODE,
    size: new Size(4 + facts.length + children.length),
    tag: elmValues.prependWords(tag),
    nFacts: facts.length,
    children,
    facts,
  };
  return nodes.prependObject(obj);
};

// ------------

const style = (key: string, value: string): FactIndex => {
  // NOTE: allocating in reverse order to arg evaluation
  const v = elmValues.prependWords(value);
  const k = elmValues.prependWords(key);
  const obj: FactStyle = {
    type: FactType.STYLE,
    key: k,
    value: v,
  };
  return facts.prependObject(obj);
};

// ------------

const ul = node('ul');
const li = node('li');

const view = ul(
  null,
  new List(
    li(new List(style('color', 'red')), new List(text('hello'))),
    new List(li(new List(style('margin', 'auto')), new List(text('world'))))
  )
);

console.log(`
nodes
=====
${nodes}
`);

console.log(`
facts
=====
${facts}
`);

console.log(`
elmValues
=========
${elmValues}
`);

checkMemoryAccessPattern(view);

/**
 * Check the memory access pattern is sequential,
 * for optimal CPU cache performance
 */
function checkMemoryAccessPattern(vdom: NodeIndex) {
  let prevNode = new NodeIndex(-1);
  let prevFact = new FactIndex(-1);
  let prevElm = new ElmIndex(-1);

  console.log('checkMemoryAccessPattern\n');

  visitNode(vdom);

  function visitNode(i: NodeIndex) {
    assert(i instanceof NodeIndex, `expected NodeIndex, got ${i}`);
    if (i.index < prevNode.index) {
      console.log(`Went backwards from ${prevNode} to ${i}`);
    } else {
      console.log(i);
    }
    prevNode = i;

    const words = nodes.words;
    let index = i.index;
    const word = words[index++];
    assert(word in NodeType);
    const type = word as NodeType;

    switch (type) {
      case NodeType.TEXT: {
        index++;
        visitElm(words[index] as any);
        return;
      }
      case NodeType.NODE: {
        const node: NodeNormal = {
          type,
          size: words[index++] as any,
          tag: words[index++] as any,
          nFacts: words[index++] as any,
          facts: [],
          children: []
        };
        const nChildren = node.size.size - 4 - node.nFacts;
        for (let c = 0; c < nChildren; c++) {
          const child: NodeIndex = words[index++] as any;
          if (!(child instanceof NodeIndex)) debugger;
          node.children.push(child);
          visitNode(child);
        }
        for (let f = 0; f < node.nFacts; f++) {
          const factIndex: FactIndex = words[index++] as any;
          if (!(factIndex instanceof FactIndex)) debugger;
          node.facts.push(factIndex);
          visitFact(factIndex);
        }
        return;
      }
      default:
        throw new Error(`Unhandled NodeType.${type}`);
    }
  }

  function visitFact(i: FactIndex) {
    assert(i instanceof FactIndex, `expected FactIndex, got ${i}`);
    if (i.index < prevFact.index) {
      console.log(`Went backwards from ${prevFact} to ${i}`);
    } else {
      console.log(i);
    }
    prevFact = i;

    const words = facts.words;
    let index = i.index;
    const word = words[index++];
    assert(word in FactType);
    const type = word as FactType;

    switch (type) {
      case FactType.STYLE: {
        visitElm(words[index++] as any);
        visitElm(words[index++] as any);
        return;
      }
      default:
        throw new Error(`Unhandled FactType.${type}`);
    }
  }

  function visitElm(i: ElmIndex) {
    assert(i instanceof ElmIndex, `expected ElmIndex, got ${i}`);
    if (i.index < prevElm.index) {
      console.log(`Went backwards from ${prevElm} to ${i}`);
    } else {
      console.log(i);
    }
    prevElm = i;
  }
}
