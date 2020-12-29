import { assert } from "console";

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
    return lines.join('\n')
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
  toString() { return `Node ${this.index}\t\t-> ${nodes.words[this.index]}`; }
}
class FactIndex extends Index {
  toString() { return `Fact ${this.index}\t\t-> ${facts.words[this.index]}`; }
}
class ElmIndex extends Index {
  toString() { return `ElmValue ${this.index}\t-> ${elmValues.words[this.index]}`; }
}

class Size {
  size: number;
  toString() { return `{size: ${this.size}}` }
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
    size: new Size(),
    valueIndex,
  };
  obj.size.size = Object.keys(obj).length;
  return nodes.prependObject(obj);
}

const node = (tag: string) => (facts: FactIndex[], children: NodeIndex[]) => {
  const obj: NodeNormal = {
    type: NodeType.NODE,
    size: new Size(),
    tag: elmValues.prependWords(tag),
    nFacts: facts.length,
    facts,
    children,
  }
  obj.size.size = Object.keys(obj).length + children.length - 1;
  return nodes.prependObject(obj);
}


// ------------

const style = (key: string, value: string): FactIndex => {
  const obj: FactStyle = {
    type: FactType.STYLE,
    key: elmValues.prependWords(key),
    value: elmValues.prependWords(value),
  }
  return facts.prependObject(obj);
};

// ------------

const ul = node('ul');
const li = node('li');

const view = ul([], [
  li([style("color", "red")], [text('hello')]),
  li([style("margin", "auto")], [text('world')]),
]);

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
=====
${elmValues}
`);
