#include "../include/BPlusTree.h"

using namespace std;

BPlusTree* BPlusTree::split() {
  BPlusTree* pa = new BPlusTree(false, NULL, maxSize);
  BPlusTree *lhc = new BPlusTree(isLeaf, pa, maxSize), *rhc = new BPlusTree(isLeaf, pa, maxSize);
  if (isLeaf) {
    for (size_t i = 0; i != keys.size() / 2; ++i) {
      //   > do NOT move key, for future comparing
      lhc->keys.push_back(keys[i]);
      //   > move or not move, either is OK
      lhc->values.push_back(values[i]);
    }
    for (size_t i = keys.size() / 2; i != keys.size(); ++i) {
      rhc->keys.push_back(keys[i]);
      rhc->values.push_back(values[i]);
    }

    if (parent != NULL) {
      size_t parent_pos = parent->find_pos(rhc->keys[0]);
      parent->keys.insert(parent->keys.begin() + parent_pos, rhc->keys[0]);
      //   delete parent->children[parent_pos]  >> delete this
      lhc->set_parent(parent);
      rhc->set_parent(parent);
      parent->children[parent_pos] = rhc;
      parent->children.insert(parent->children.begin() + parent_pos, lhc);
      //pa->children.clear();
      //   > free temparory parent node pointer
      delete pa;
      return parent;
    }
    else {
      pa->keys.push_back(rhc->keys[0]);
      pa->children.push_back(lhc);
      pa->children.push_back(rhc);
      return pa;
    }
  }
  // split nonleaf node
  else {
    for (size_t i = 0; i != keys.size() /2; i++) {
      //   > reserve key for future comparing
      lhc->keys.push_back(keys[i]);
      //   > move pointer
      //   > keep memory from freed
      lhc->children.push_back(children[i]);
      children[i] = NULL;
      lhc->children[i]->set_parent(lhc);
    }
    lhc->children.push_back(children[keys.size() / 2]);
    children[keys.size() / 2] = NULL;
    lhc->children[keys.size() / 2]->set_parent(lhc);

    for (size_t i = keys.size() / 2 + 1; i != keys.size(); i++) {
      rhc->keys.push_back(keys[i]);
      rhc->children.push_back(children[i]);
      children[i] = NULL;
      rhc->children[i - keys.size() / 2 - 1]->set_parent(rhc);
    }
    rhc->children.push_back(children[keys.size()]);
    children[keys.size()] = NULL;
    rhc->children[keys.size() - keys.size() / 2 - 1]->set_parent(rhc);

    if (parent != NULL) {
      int& tmp = keys[keys.size() / 2];
      size_t parent_pos = parent->find_pos(tmp);
      parent->keys.insert(parent->keys.begin() + parent_pos, tmp);
      rhc->set_parent(parent);
      parent->children[parent_pos] = rhc;
      lhc->set_parent(parent);
      parent->children.insert(parent->children.begin() + parent_pos, lhc);
      delete pa;
      return parent;
    }
    else {
      pa->keys.push_back(keys[keys.size() / 2]);
      pa->children.push_back(lhc);
      pa->children.push_back(rhc);
      return pa;
    }
  }
}

BPlusTree* BPlusTree::bm() {
  size_t parent_pos = 0;
  while (parent_pos != parent->children.size() && parent->children[parent_pos] != this) {
    parent_pos++;
  }
  if (parent_pos == 0) {
    BPlusTree* &right = parent->children[parent_pos + 1];
    if (right->keys.size() > maxSize / 2) {
      return borrow_right(right, parent_pos);
    }
    else
      return merge_right(right, parent_pos);
  }
  else if (parent_pos == parent->children.size() - 1) {
    BPlusTree* &left = parent->children[parent_pos - 1];
    if (left->keys.size() > maxSize / 2) {
      return borrow_left(left, parent_pos-1);
    }
    else
      return merge_left(left, parent_pos - 1);
  }
  else {
    BPlusTree* &left = parent->children[parent_pos - 1];
    BPlusTree* &right = parent->children[parent_pos + 1];
    if (left->keys.size() > right->keys.size()) {
      if (left->keys.size() > maxSize / 2) {
        return borrow_left(left, parent_pos - 1);
      }
      else
        return merge_left(left, parent_pos-1);
    }
    else
      if (right->keys.size() > maxSize / 2) {
        return borrow_right(right, parent_pos);
      }
      else
        return merge_right(right, parent_pos);
  }
}

//WARN: change the parent pointer when move pointer in children
BPlusTree* BPlusTree::borrow_left(BPlusTree* &left, const size_t& parent_pos) {
  if (isLeaf) {
    keys.insert(keys.begin(), *(left->keys.end() - 1));
    values.insert(values.begin(), *(left->values.end() - 1));
    left->keys.erase(left->keys.end() - 1);
    left->values.erase(left->values.end() - 1);
    parent->keys[parent_pos] = keys[0];
    return this;
  }
  else {
    keys.insert(keys.begin(), parent->keys[parent_pos]);
    children.insert(children.begin(), std::move(*(left->children.end() - 1)));
    *(left->children.end() - 1) = NULL;
    left->children.erase(left->children.end() - 1);
    children[0]->set_parent(this);
    parent->keys[parent_pos] = *(left->keys.end() - 1);
    left->keys.erase(left->keys.end() - 1);
    return this;
  }
}

//change the parent pointer when move pointer in children
BPlusTree* BPlusTree::merge_left(BPlusTree* &left, const size_t& parent_pos) {
  if (isLeaf) {
    vector<int> newkeys;
    vector<SequenceSet*> newvalues;

    //   > move keys and values
    for (size_t i = 0; i != left->keys.size(); i++) {
      newkeys.push_back(left->keys[i]);
      newvalues.push_back(left->values[i]);
    }
    for (size_t i = 0; i != keys.size(); i++) {
      newkeys.push_back(keys[i]);
      newvalues.push_back(values[i]);
    }

    //   > replace keys and values
    this->keys = std::move(newkeys);
    this->values = std::move(newvalues);

    //   > free left node and delete left node from parent's children
    delete left;
    parent->children[parent_pos] = NULL;
    parent->children.erase(parent->children.begin() + parent_pos);

    //   > delete specified key in parent's keys
    parent->keys.erase(parent->keys.begin() + parent_pos);
    return this;
  }
  else {
    vector<int> newkeys;
    vector<BPlusTree*> newchildren;

    //   > move key and children
    for (size_t i = 0; i != left->keys.size(); ++i) {
      newkeys.push_back(left->keys[i]);
      //   -> set parent
      //   -> move
      //   -> reset pointer to NULL
      left->children[i]->set_parent(this);
      newchildren.push_back(left->children[i]);
      left->children[i] = NULL;
    }
    left->children[left->keys.size()]->set_parent(this);
    newchildren.push_back(left->children[left->keys.size()]);
    left->children[left->keys.size()] = NULL;

    //   > free left and delete it from parent's children
    delete left;
    parent->children[parent_pos] = NULL;
    parent->children.erase(parent->children.begin() + parent_pos);

    //   > move parent's specified key
    newkeys.push_back(parent->keys[parent_pos]);
    parent->keys.erase(parent->keys.begin() + parent_pos);

    //   > append pre-existed key and children to the new vec
    for (size_t i = 0; i != keys.size(); ++i) {
      newkeys.push_back(keys[i]);
      newchildren.push_back(children[i]);
      children[i] = NULL;
    }
    newchildren.push_back(children[keys.size()]);
    children[keys.size()] = NULL;

    //   > replace
    keys = std::move(newkeys);
    children = std::move(newchildren);
    return this;
  }
}

//change the parent pointer when move pointer in children
BPlusTree* BPlusTree::borrow_right(BPlusTree* &right, const size_t& parent_pos) {
  if (isLeaf) {
    keys.push_back(right->keys[0]);
    values.push_back(right->values[0]);
    right->keys.erase(right->keys.begin());
    right->values.erase(right->values.begin());
    parent->keys[parent_pos] = right->keys[0];
    return this;
  }
  else {
    keys.push_back(parent->keys[parent_pos]);
    right->children[0]->set_parent(this);
    children.push_back(right->children[0]);
    right->children[0] = NULL;
    right->children.erase(right->children.begin());
    parent->keys[parent_pos] = right->keys[0];
    right->keys.erase(right->keys.begin());
    return this;
  }
}

//change the parent pointer when move pointer in children
BPlusTree* BPlusTree::merge_right(BPlusTree* &right, const size_t& parent_pos) {
  if (isLeaf) {
    parent->keys.erase(parent->keys.begin() + parent_pos);
    for (size_t i = 0; i != right->keys.size(); ++i) {
      keys.push_back(right->keys[i]);
      values.push_back(right->values[i]);
    }
    delete right;
    parent->children[parent_pos + 1] = NULL;
    parent->children.erase(parent->children.begin() + parent_pos + 1);
    return this;
  }
  else {
    keys.push_back(parent->keys[parent_pos]);
    parent->keys.erase(parent->keys.begin() + parent_pos);
    for (size_t i = 0; i != right->keys.size(); i++) {
      keys.push_back(right->keys[i]);

      right->children[i]->set_parent(this);
      children.push_back(std::move(right->children[i]));
      right->children[i] = NULL;
    }
    right->children[right->keys.size()]->set_parent(this);
    children.push_back(std::move(right->children[right->keys.size()]));
    right->children[right->keys.size()] = NULL;

    delete right;
    parent->children[parent_pos + 1] = NULL;
    parent->children.erase(parent->children.begin() + parent_pos + 1);
    return this;
  }
}

size_t BPlusTree::find_pos(const int& key) {
  size_t i = 0;
  while (i != keys.size() && key >= keys[i]) {
    ++i;
  }
  return i;
}

size_t BPlusTree::find_pos(const int& key) const {
  size_t i = 0;
  while (i != keys.size() && key > keys[i]) {
    ++i;
  }
  return i;
}

//   > private method
void BPlusTree::remove(const size_t& pos) {
  if (!isLeaf) throw runtime_error("this is a nonleaf node. Action refused");
  keys.erase(keys.begin() + pos);
  values.erase(values.begin() + pos);
}

BPlusTree::BPlusTree(const bool& isleaf, BPlusTree* p, const int& size)
  : isLeaf(isleaf), parent(p), maxSize(size) {
  if (maxSize < 3)
    throw runtime_error("B+Tree max size too small");
}

BPlusTree::BPlusTree(const bool& isleaf, const int& size)
  : isLeaf(isleaf), parent(NULL), maxSize(size) {
  if (maxSize < 3)
    throw runtime_error("B+Tree max size too small");
}

BPlusTree::BPlusTree(const BPlusTree& other) : isLeaf(other.isLeaf), parent(other.parent),
                                               keys(other.keys), children(other.children), values(other.values), maxSize(3) {
  if (maxSize < 3)
    throw runtime_error("B+Tree max size too small");
}

BPlusTree& BPlusTree::operator=(const BPlusTree& rhs) {
  maxSize = rhs.maxSize;
  isLeaf = rhs.isLeaf;
  parent = rhs.parent;
  keys = rhs.keys;
  children = rhs.children;
  values = rhs.values;
  return *this;
}

BPlusTree::~BPlusTree() {
  for (vector<BPlusTree*>::iterator it = children.begin(); it != children.end(); it++) {
    delete *it;
  }
  parent = NULL;
}

void BPlusTree::set_parent(BPlusTree* p) {
  parent = p;
}

void BPlusTree::set_max_size(const int& size) {
  if (size < 3)
    throw runtime_error("B+Tree max size too small");
  maxSize = size;
}

SequenceSet* BPlusTree::leftest() {
  if (isLeaf) {
    if (values.empty()) return NULL;
    else return *values.begin();
  }
  else {
    return (*children.begin())->leftest();
  }
}

int BPlusTree::successor(const int& curr) {
  if (isLeaf) {
    vector<int>::iterator it = keys.begin();
    for (; it != keys.end(); ++it) {
      if (*it == curr) break;
    }
    if (it == keys.end()) {
      throw runtime_error(curr + " Not found.");
    }
    //   > curr is the last element in content
    if (it + 1 == keys.end()) {
      BPlusTree* curr = this;
      BPlusTree* up = parent;
      size_t pos = 0;
      //   > if parent doesn't exist
      if (up == NULL) return -1;

      for (; pos != up->children.size(); ++pos) {
        if (up->children[pos] == curr) break;
      }
      while (pos == up->keys.size()) {
        curr = up;
        up = up->parent;
        if (up == NULL) return -1;
        pos = 0;
        for (; pos != up->children.size(); ++pos) {
          if (up->children[pos] == curr) break;
        }
      }
      return up->keys[pos];
    }
    //   > curr is not the last one
    else {
      return *(it + 1);
    }
  }
  else {
    size_t pos = 0;
    for (; pos != keys.size(); ++pos) {
      if (keys[pos] > curr) break;
    }
    return children[pos]->successor(curr);
  }
}

bool BPlusTree::find(const int& key) const {
  if (isLeaf) {
    for (vector<int>::const_iterator it = keys.cbegin(); it != keys.cend(); ++it) {
      if (*it == key) return true;
    }
    return false;
  }
  else {
    size_t pos = find_pos(key);
    return children[pos]->find(key);
  }
}

SequenceSet* BPlusTree::search(const int& key) {
  if (isLeaf) {
    for (size_t i = 0; i != keys.size(); i++) {
      if (keys[i] == key) return values[i];
    }
    throw runtime_error(key + " not found");
  }
  else {
    size_t pos = find_pos(key);
    return children[pos]->search(key);
  }
}

BPlusTree* BPlusTree::insert(const int& key, SequenceSet* value) {
  if (isLeaf) {
    size_t pos = find_pos(key);
    keys.insert(keys.begin() + pos, key);
    values.insert(values.begin() + pos, value);
    if (keys.size() >= maxSize) {
      BPlusTree* ret = split();
      delete this;
      return ret;
    }
    return this;
  }
  else {
    size_t pos = find_pos(key);
    children[pos]->insert(key, value);
    if (keys.size() >= maxSize) {
      BPlusTree* ret = split();
      delete this;
      return ret;
    }
    return this;
  }
}

BPlusTree* BPlusTree::del(const int& key) {
  if (isLeaf) {
    size_t pos = find_pos(key);
    if (pos <= 0 || pos > keys.size())
      throw runtime_error("key doesn't exist. Delete refused");
    pos--;
    if (keys[pos] != key)
      throw runtime_error("key doesn't exist. Delete refused");
    remove(pos);
    if (parent != NULL && keys.size() < maxSize / 2) {
      return bm();
    }
    else return this;
  }
  else {
    size_t pos = find_pos(key);
    if (pos > 0 && pos <= keys.size()) {
      if (key == keys[pos - 1]) {
        keys[pos - 1] = successor(key);
      }
    }
    BPlusTree* ret = children[pos]->del(key);
    if (parent != NULL && keys.size() < maxSize / 2) {
      return bm();
    }
    else return this;
  }
}

bool BPlusTree::empty() {
  return keys.empty();
}

BPlusTree* BPlusTree::only_child() {
  return children[0];
}


string BPlusTree::toString() {
  if (isLeaf) {
    ostringstream oss;
    //oss << '|';
    for (size_t i = 0; i != values.size(); ++i) {
      oss << values[i]->toString() << "  ";
    }
    //oss << '|';
    return oss.str();
  }
  else {
    ostringstream oss;
    //oss << '|';
    for (vector<BPlusTree*>::iterator it = children.begin(); it != children.end(); ++it) {
      oss << (*it)->toString() << "*";
    }
    //oss << '|';
    return oss.str();
  }
}

BPlusTree* inherit(BPlusTree* & _root) {
  if (_root->empty()) {
    _root->children[0]->set_parent(NULL);
    BPlusTree* ret = _root->children[0];
    _root->children[0] = NULL;
    delete _root;
    return ret;
  }
  return _root;
}

BPlusTree* rebuildTree(BPlusTree* prevTree) {
  BPlusTree* newTree = new BPlusTree(true, NULL, prevTree->getSize() * 2);
  std::vector<int> keysList = prevTree->getKeys();
  for(std::vector<int>::iterator i = keysList.begin(); i != keysList.end(); i++) {
    int currentKey = *(i);
    newTree = newTree->insert(currentKey, prevTree->search(currentKey));
  }
  return newTree;
}

BPlusTree* rebuildTree(BPlusTree* prevTree, int order) {
  BPlusTree* newTree = new BPlusTree(true, NULL, order);
  std::vector<int> keysList = prevTree->getKeys();
  for(std::vector<int>::iterator i = keysList.begin(); i != keysList.end(); i++) {
    int currentKey = *(i);
    newTree = newTree->insert(currentKey, prevTree->search(currentKey));
  }
  return newTree;
}
