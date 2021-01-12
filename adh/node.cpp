#include "node.h"

engine::adh::Node::Node()
{
}

engine::adh::Node::Node(const std::string & name):
  _name(name)
{
}

void engine::adh::Node::draw(Context & context) const
{
  for(auto && child : _children)
  {
    child->draw(context);
  }
}

void engine::adh::Node::addChild(const std::shared_ptr<Node> & child)
{
  _children.push_back(child);
}
