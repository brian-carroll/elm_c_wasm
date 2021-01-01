import { assert } from 'console';

enum VdomCtor {
  NODE = 'NODE',
  NODE_TEXT = 'NODE_TEXT',
  KEYED_NODE = 'KEYED_NODE',
  NODE_TAGGER = 'NODE_TAGGER',
  NODE_THUNK = 'NODE_THUNK',
  // NODE_CUSTOM = 'NODE_CUSTOM', // unused in elm org libs!
  FACT_EVENT = 'FACT_EVENT',
  FACT_STYLE = 'FACT_STYLE',
  FACT_PROP = 'FACT_PROP',
  FACT_ATTR = 'FACT_ATTR',
  FACT_ATTR_NS = 'FACT_ATTR_NS'
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
    return `Size ${this.size}`;
  }
}

class Cons<T> {
  constructor(public head: T, public tail: List<T> = null) {}
}
type List<T> = Cons<T> | null;

const nodes = new VdomArray<NodeIndex, NodeWord, VdomNode>(new NodeIndex(100));
const facts = new VdomArray<FactIndex, FactWord, VdomFact>(new FactIndex(100));
const elmValues = new VdomArray<ElmIndex, any, any>(new ElmIndex(100));

type NodeWord = VdomCtor | number | ElmIndex | NodeIndex | FactIndex;
type FactWord = VdomCtor | ElmIndex;

// 3 non-index values
interface NodeText {
  type: VdomCtor.NODE_TEXT;
  size: Size;
  valueIndex: ElmIndex;
}

// 3 non-index values
interface NodeNormal {
  type: VdomCtor.NODE;
  size: Size;
  nFacts: number;
  tag: ElmIndex;
  facts: FactIndex[];
  children: NodeIndex[];
}

interface NodeNS extends NodeNormal {
  namespace: ElmIndex;
}

// 3 non-index values
interface NodeKeyed {
  type: VdomCtor.KEYED_NODE;
  size: Size;
  nFacts: number;
  tag: ElmIndex;
  namespace: ElmIndex;
  facts: FactIndex[];
  keysBegin: ElmIndex;
  children: NodeIndex[];
}

interface NodeKeyedNS extends NodeKeyed {
  namespace: ElmIndex;
}

// 2 non-index values
interface NodeTagger {
  type: VdomCtor.NODE_TAGGER;
  size: Size;
  tagger: ElmIndex;
  nodeIndex: NodeIndex;
}

// 2 non-index values
interface NodeThunk {
  type: VdomCtor.NODE_THUNK;
  size: Size;
  nodeIndex: NodeIndex;
  thunk: ElmIndex;
  refs: ElmIndex[];
}

type VdomNode = NodeText | NodeNormal | NodeKeyed | NodeTagger | NodeThunk;

// 1 non-index value
interface FactEvent {
  type: VdomCtor.FACT_EVENT;
  key: ElmIndex;
  handler: ElmIndex;
}

// 1 non-index value
interface FactStyle {
  type: VdomCtor.FACT_STYLE;
  key: ElmIndex;
  value: ElmIndex;
}

// 1 non-index value
interface FactProp {
  type: VdomCtor.FACT_PROP;
  key: ElmIndex;
  value: ElmIndex;
}

// 1 non-index value
interface FactAttr {
  type: VdomCtor.FACT_ATTR;
  key: ElmIndex;
  value: ElmIndex;
}

// 1 non-index value
interface FactAttrNS {
  type: VdomCtor.FACT_ATTR_NS;
  key: ElmIndex;
  value: ElmIndex;
  namespace: ElmIndex;
}

type VdomFact = FactEvent | FactStyle | FactProp | FactAttr | FactAttrNS;

const text = (content: string): NodeIndex => {
  const valueIndex = elmValues.prependWords(content);
  const obj: NodeText = {
    type: VdomCtor.NODE_TEXT,
    size: new Size(3),
    valueIndex
  };
  return nodes.prependObject(obj);
};

const node = (tag: string) => (
  factList: List<FactIndex>,
  kidList: List<NodeIndex>
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
    type: VdomCtor.NODE,
    size: new Size(4 + facts.length + children.length),
    tag: elmValues.prependWords(tag),
    nFacts: facts.length,
    children,
    facts
  };
  return nodes.prependObject(obj);
};

// ------------

const style = (key: string, value: string): FactIndex => {
  // NOTE: allocating in reverse order to arg evaluation
  const v = elmValues.prependWords(value);
  const k = elmValues.prependWords(key);
  const obj: FactStyle = {
    type: VdomCtor.FACT_STYLE,
    key: k,
    value: v
  };
  return facts.prependObject(obj);
};

function checkMemoryAccessPattern(vdom: NodeIndex) {
  const nodeSeq: NodeIndex[] = [];
  const factSeq: FactIndex[] = [];
  const elmSeq: ElmIndex[] = [];

  visitNode(vdom);

  console.log(`
Memory access patterns

For the most common Elm coding pattern, Vdom values will
end up being arranged in memory in tree traversal order.
That should give us the fastest diff on modern hardware,
where memory access is the bottleneck.

3 arrays in Vdom structure-of-arrays:

${nodeSeq.join('\n')}

${factSeq.join('\n')}

${elmSeq.join('\n')}
  `);

  function visitNode(i: NodeIndex) {
    assert(i instanceof NodeIndex, `expected NodeIndex, got ${i}`);
    const prevNode = nodeSeq[nodeSeq.length-1];
    if (prevNode && i.index < prevNode.index) {
      console.log(`Went backwards from ${prevNode} to ${i}`);
    } else {
      console.log(i);
    }
    nodeSeq.push(i);

    const words = nodes.words;
    let index = i.index;
    const word = words[index++];
    assert(word in VdomCtor);
    const type = word as VdomCtor;

    switch (type) {
      case VdomCtor.NODE_TEXT: {
        index++;
        visitElm(words[index] as any);
        return;
      }
      case VdomCtor.NODE: {
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
    const prevFact = factSeq[factSeq.length-1];
    if (prevFact && i.index < prevFact.index) {
      console.log(`Went backwards from ${prevFact} to ${i}`);
    } else {
      console.log(i);
    }
    factSeq.push(i);

    const words = facts.words;
    let index = i.index;
    const word = words[index++];
    assert(word in VdomCtor);
    const type = word as VdomCtor;

    switch (type) {
      case VdomCtor.FACT_STYLE: {
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
    const prevElm = elmSeq[elmSeq.length-1];
    if (prevElm && i.index < prevElm.index) {
      console.log(`Went backwards from ${prevElm} to ${i}`);
    } else {
      console.log(i);
    }
    elmSeq.push(i);
  }
}

// ------------

const ul = node('ul');
const li = node('li');

const view = ul(
  null,
  new Cons(
    li(
      new Cons(style('color', 'red'), new Cons(style('padding', '10px'))),
      new Cons(text('hello'), new Cons(text('there')))
    ),
    new Cons(
      li(
        new Cons(style('margin', 'auto'), new Cons(style('float', 'left'))),
        new Cons(text('world'), new Cons(text('people')))
      ),
      new Cons(
        li(
          new Cons(style('color', 'blue'), new Cons(style('float', 'right'))),
          new Cons(text(`what's`), new Cons(text('up?')))
        )
      )
    )
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
