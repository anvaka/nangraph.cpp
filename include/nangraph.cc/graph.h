#ifndef GRAPH_H
#define GRAPH_H

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <set>

class Graph;

class Node {
private:
  // these sets store all outgoing and incoming edges for a given node.
  // As you see this brings us to the world of oriented graphs. I'm not sure
  // if I'm going to add non-oriented graphs yet.
  std::multiset<std::size_t> *inNodes, *outNodes;
public:

  Node() {
    inNodes = nullptr;
    outNodes = nullptr;
  }

  ~Node() {
    if (inNodes != nullptr) delete inNodes;
    if (outNodes != nullptr) delete outNodes;
  }

  friend class Graph;

  int degree() {
    return inDegree() + outDegree();
  }

  int inDegree() {
    return inNodes == nullptr ? 0 : inNodes->size();
  }

  int outDegree() {
    return outNodes == nullptr ? 0 : outNodes->size();
  }

  void addOutLink(std::size_t id) {
    if (outNodes == nullptr) outNodes = new std::multiset<std::size_t>();
    outNodes->insert(id);
  }

  void addInLink(std::size_t id) {
    if (inNodes == nullptr) inNodes = new std::multiset<std::size_t>();
    inNodes->insert(id);
  }

};


typedef std::function<bool(const std::size_t&)> NodeCallback;
typedef std::function<bool(const std::size_t&, const std::size_t&, const std::size_t&)> LinkCallback;
typedef std::function<bool(const std::size_t&, const std::size_t &)> NodeLinkCallback;

/**
 * This is our core graph structure. I'm trying to optimize for RAM and speed.
 *
 * Targets:
 * Node insertion time: O(1);
 * Node removal time: O(m * lg(m)), where m is a max degree of adjacent nodes.
 */
class Graph {

public:

private:
  // maps a node id to Node structure.
  std::unordered_map<std::size_t, Node*> _nodes;

  int _linksCount = 0;

public:
  Graph();
  ~Graph();

  Node* addNode(const std::size_t& id);
  Node* getNode(const std::size_t& id);

  std::size_t addLink(const std::size_t& fromId, const std::size_t &toId);
  std::size_t getLinkId(const std::size_t& fromId, const std::size_t &toId);

  bool hasLink(const std::size_t& fromId, const std::size_t &toId);
  int getNodesCount() { return _nodes.size(); }
  int getLinksCount() { return _linksCount; }

  bool forEachNode(NodeCallback callback);
  bool forEachLink(LinkCallback callback);
  bool forEachLinkedNode(const std::size_t fromId, bool isOut, NodeLinkCallback callback);
};

#endif
