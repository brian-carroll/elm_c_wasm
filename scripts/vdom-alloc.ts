enum NodeType {
  TEXT,
  NODE,
  KEYED_NODE,
  // CUSTOM,
  TAGGER,
  THUNK
}

enum FactType {
  EVENT,
  STYLE,
  PROP,
  ATTR,
  ATTR_NS
}

class NodeIndex {
  constructor(public value: number) {}
}

class FactIndex {
  constructor(public value: number) {}
}

class ElmIndex {
  constructor(public value: number) {}
}

interface NodeText {
  type: NodeType.TEXT;
  size: number;
  valueIndex: ElmIndex;
}

interface NodeNormal {
  type: NodeType.NODE;
  size: number;
  namespace: ElmIndex;
  factsBegin: FactIndex;
  factsEnd: FactIndex;
  children: NodeIndex[];
}

interface NodeKeyed {
  type: NodeType.KEYED_NODE;
  size: number;
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
