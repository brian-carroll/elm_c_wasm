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

class Index {
  constructor(public value: number) {}
}

class NodeIndex extends Index { }
class FactIndex extends Index { }
class ElmIndex extends Index { }

type VdomNode = NodeText | NodeNormal | NodeKeyed | NodeTagger | NodeThunk;

interface NodeText {
  type: NodeType.TEXT;
  size: number;
  valueIndex: ElmIndex;
}

interface NodeNormal {
  type: NodeType.NODE;
  size: number;
  tag: ElmIndex;
  namespace: ElmIndex;
  factsBegin: FactIndex;
  factsEnd: FactIndex;
  children: NodeIndex[];
}

interface NodeKeyed {
  type: NodeType.KEYED_NODE;
  size: number;
  tag: ElmIndex;
  namespace: ElmIndex;
  factsBegin: FactIndex;
  factsEnd: FactIndex;
  keysBegin: ElmIndex;
  children: NodeIndex[];
}

interface NodeTagger {
  type: NodeType.TAGGER;
  size: number;
  tagger: ElmIndex;
  nodeIndex: NodeIndex;
}

interface NodeThunk {
  type: NodeType.THUNK;
  size: number;
  nodeIndex: NodeIndex;
  thunk: ElmIndex;
  refs: ElmIndex[];
}

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


type NodeWord = undefined | NodeType | number | ElmIndex | NodeIndex | FactIndex;
type FactWord = undefined | FactType | ElmIndex;


class VdomArray<T, I extends Index> {
  private data: T[];
  public bottom: I;

  constructor(size: I) {
    this.data = new Array<T>(size.value);
    size.value--;
    this.bottom = size;
  }

  prepend(words: T[]): I {
    let newBottom = this.bottom.value - words.length;
    if (newBottom < 0) {
      console.warn('resizing', this.bottom);
      const oldData = this.data;
      const oldLen = oldData.length;
      this.data = new Array<T>(oldLen * 2);
      oldData.forEach((word, i) => {
        this.data[oldLen + i] = word;
      });
      newBottom += oldLen;
    }

    words.forEach((word, i) => {
      this.data[newBottom + i] = word;
    });

    this.bottom.value = newBottom;
    return this.bottom;
  }
}

const nodes = new VdomArray<NodeWord, NodeIndex>(new NodeIndex(100));
const facts = new VdomArray<FactWord, FactIndex>(new FactIndex(100));
const elmValues = new VdomArray<any, ElmIndex>(new ElmIndex(100));
