#include <iostream>
#include <limits.h>
#include <functional>


struct SplayTree
{
private:
    struct Node;
    
    Node* _root;
    
    void _Rotate(Node*, bool is_left);
    void _RightRotate(Node*);
    void _LeftRotate(Node*);
    void _Splay(Node*);
    void _Transplant(Node*, Node*);
    Node* _FindVertex(Node*, size_t);
    Node* _Merge(Node*, Node*);
    std::pair<Node*, Node*> _Split(Node*, size_t);
    void _PermutationOnSubsegment(Node*&, bool);
    void _SubsegmentOperation(size_t, size_t);
    
    template <typename T>
    T _SubsegmentOperation(size_t begin, size_t end, std::function<T(Node*&)> func);
    
    inline void _ZigMove(Node*);
    inline void _ZigZigMove(Node*);
    inline void _ZigZagMove(Node*);
    
    static void DFS(Node*, std::ostream&);
    static void DeleteTree(Node*);
    
public:
    SplayTree() : _root(nullptr) {}
    ~SplayTree();
    
    void Insert(const long long&, size_t);
    void Delete(size_t);
    long long GetSummOnSubsegment(size_t, size_t);
    void SetElemOnSubsegment(const long long&, size_t, size_t);
    void AddElemOnSubsegment(const long long&, size_t, size_t);
    void NextPermutationOnSubsegment(size_t, size_t);
    void PrevPermutationOnSubsegment(size_t, size_t);
    
    void PrintTree(std::ostream&);
};

struct SplayTree::Node
{
    long long data;               // Data in this vertex
    size_t size;                  // Size of Tree
    long long summ;               // Summ on Tree
    long long min;                // Minimum element on Tree
    long long max;                // Maximum element on Tree
    Node* left;                   // Pointer to left vertex
    Node* right;                  // Pointer to right vertex
    Node* parent;                 // Pointer to parent vertex
    
    bool _set;                    // For set number on Tree
    long long _updateSet;         // For set number on Tree
    bool _add;                    // For add number on Tree
    long long _updateAdd;         // For add number on Tree
    bool _reverse;                // For reverse Tree
    
    size_t _upperSuffix;
    size_t _lowerSuffix;
    size_t _upperPrefix;
    size_t _lowerPrefix;
    
    long long _LeftVertex;         // The most left vertex on Tree
    long long _RightVertex;        // The most right vertex on Tree
    
    
    Node() : data(0), size(1), summ(0), min(0), max(0), left(nullptr), right(nullptr), parent(nullptr),
                        _set(false), _updateSet(0), _add(false), _updateAdd(0), _reverse(false),
                        _upperSuffix(1), _lowerSuffix(1), _upperPrefix(1), _lowerPrefix(1),
                        _LeftVertex(0), _RightVertex(0) {}
    
    Node(long long element, size_t size = 1, long long summ = 0, Node* left = nullptr, Node* right = nullptr, Node* parent = nullptr) :
                        data(element), size(size), summ(summ), min(element), max(element), left(left), right(right), parent(parent),
                        _set(false), _updateSet(0), _add(false), _updateAdd(0), _reverse(false),
                        _upperSuffix(1), _lowerSuffix(1), _upperPrefix(1), _lowerPrefix(1),
                        _LeftVertex(element), _RightVertex(element) {}
    
    Node(const Node& rhs) : data(rhs.data), size(rhs.size), summ(rhs.summ), min(rhs.min), max(rhs.max), left(rhs.left), right(rhs.right), parent(rhs.parent),
                        _set(false), _updateSet(0), _add(false), _updateAdd(0), _reverse(false),
                        _upperSuffix(rhs._upperSuffix), _lowerSuffix(rhs._lowerSuffix),
                        _upperPrefix(rhs._upperPrefix), _lowerPrefix(rhs._lowerPrefix),
                        _LeftVertex(rhs._LeftVertex), _RightVertex(rhs._RightVertex) {}
    ~Node() = default;
    
    enum Side {
        Left,
        Right
    };
    
    void SetChild(Node*, Side);
    
    static void Update(Node*);
    static void Push(Node*);
    
    template <typename V>
    static V GetField(Node*, V Node::*, V default_value);
    
    static void SetFlag_SetElem(Node*, const long long&);
    static void SetFlag_AddElem(Node*, const long long&);
    static void SetFlag_Reverse(Node*);
};

SplayTree::~SplayTree()
{
    DeleteTree(_root);
}

void SplayTree::DeleteTree(Node* vertex)
{
    if (vertex == nullptr) {
        return;
    }
    
    DeleteTree(vertex->left);
    DeleteTree(vertex->right);
    
    delete vertex;
}

void SplayTree::DFS(Node* vertex, std::ostream& fout)
{
    if (vertex == nullptr) {
        return;
    }
    
    Node::Push(vertex);
    Node::Update(vertex);
    
    DFS(vertex->left, fout);
    fout << vertex->data << " ";
    DFS(vertex->right, fout);
}

void SplayTree::Node::SetChild(Node* new_node, Node::Side child_side)
{
    if (child_side == Right) {
        right = new_node;
    } else {
        left = new_node;
    }
    if (new_node != nullptr) {
        new_node->parent = this;
    }
}

void SplayTree::Node::Update(Node* vertex)
{
    if (vertex == nullptr) {
        return;
    }
    
    Push(vertex->left);
    Push(vertex->right);
    
    vertex->size = GetField<size_t>(vertex->left, &Node::size, 0u) + GetField<size_t>(vertex->right, &Node::size, 0u) + 1;
    
    vertex->summ = GetField<long long>(vertex->left, &Node::summ, 0) +
                   GetField<long long>(vertex->right, &Node::summ, 0) +
                   vertex->data;
    
    vertex->min = std::min(std::min(GetField<long long>(vertex->left, &Node::min, INT_MAX),
                                    GetField<long long>(vertex->right, &Node::min, INT_MAX)),
                                    vertex->data);
    vertex->max = std::max(std::max(GetField<long long>(vertex->left, &Node::max, INT_MIN),
                                    GetField<long long>(vertex->right, &Node::max, INT_MIN)),
                                    vertex->data);
    
    vertex->_LeftVertex = (vertex->left == nullptr ? vertex->data : GetField<long long>(vertex->left, &Node::_LeftVertex, 0));
    vertex->_RightVertex = (vertex->right == nullptr ? vertex->data : GetField<long long>(vertex->right, &Node::_RightVertex, 0));
    
    auto func = [&vertex](size_t Node::* member, Node* Node::* first, Node* Node::* second, long long Node::* left, long long Node::* right, std::function<bool(long long, long long)> comp) {
        
        vertex->*member = GetField<size_t>(vertex->*first, member, 0);
        if (GetField<size_t>(vertex->*first, member, 0) == GetField<size_t>(vertex->*first, &Node::size, 0u)) {
            if (vertex->*first == nullptr || comp(vertex->data, GetField<long long>(vertex->*first, left, 0))) {
                (vertex->*member)++;
                if (vertex->*second != nullptr && comp(GetField<long long>(vertex->*second, right, 0), vertex->data)) {
                    vertex->*member += GetField<size_t>(vertex->*second, member, 0);
                }
            }
        }
    };
    
    func(&Node::_lowerSuffix, &Node::right, &Node::left, &Node::_LeftVertex, &Node::_RightVertex, [](long long lhs, long long rhs){ return lhs >= rhs; });
    
    func(&Node::_upperSuffix, &Node::right, &Node::left, &Node::_LeftVertex, &Node::_RightVertex, [](long long lhs, long long rhs){ return lhs <= rhs; });

    func(&Node::_lowerPrefix, &Node::left, &Node::right, &Node::_RightVertex, &Node::_LeftVertex, [](long long lhs, long long rhs){ return lhs <= rhs; });
    
    func(&Node::_upperPrefix, &Node::left, &Node::right, &Node::_RightVertex, &Node::_LeftVertex, [](long long lhs, long long rhs){ return lhs >= rhs; });
}

template <typename V>
V SplayTree::Node::GetField(Node* vertex, V Node::* member, V default_value)
{
    return (vertex ? vertex->*member : default_value);
}

void SplayTree::Node::SetFlag_SetElem(Node* vertex, const long long& elem)
{
    if (vertex == nullptr) {
        return;
    }
    
    vertex->_add = false;
    vertex->_updateAdd = 0;
    
    vertex->_set = true;
    vertex->_updateSet = elem;
    
    Push(vertex);
}

void SplayTree::Node::SetFlag_AddElem(Node* vertex, const long long& elem)
{
    if (vertex == nullptr) {
        return;
    }
    
    vertex->_add = true;
    vertex->_updateAdd += elem;
    
    Push(vertex);
}

void SplayTree::Node::SetFlag_Reverse(Node* vertex)
{
    if (vertex == nullptr) {
        return;
    }
    
    vertex->_reverse ^= true;
    
    Push(vertex);
}


void SplayTree::Node::Push(Node* vertex)
{
    if (vertex == nullptr) {
        return;
    }
    
    // Do changes in vertex
    
    if (vertex->_set) {
        vertex->data = vertex->min = vertex->max = vertex->_updateSet;
        vertex->summ = vertex->_updateSet * static_cast<long long>(vertex->size);
        vertex->_RightVertex = vertex->_LeftVertex = vertex->_updateSet;
        vertex->_lowerSuffix = vertex->_upperSuffix = vertex->size;
        vertex->_lowerPrefix = vertex->_upperPrefix = vertex->size;
    }
    
    if (vertex->_add) {
        vertex->data += vertex->_updateAdd;
        vertex->summ += vertex->_updateAdd * static_cast<long long>(vertex->size);
        vertex->min += vertex->_updateAdd;
        vertex->max += vertex->_updateAdd;
        vertex->_LeftVertex += vertex->_updateAdd;
        vertex->_RightVertex += vertex->_updateAdd;
    }
    
    if (vertex->_reverse) {
        std::swap(vertex->_upperPrefix, vertex->_lowerSuffix);
        std::swap(vertex->_lowerPrefix, vertex->_upperSuffix);
        std::swap(vertex->_LeftVertex, vertex->_RightVertex);
        std::swap(vertex->left, vertex->right);
    }
    
    auto PushHelp = [vertex](Node* curr_v) {
        if (curr_v == nullptr) {
            return;
        }
        
        if (vertex->_set) {
            curr_v->_add = false;
            curr_v->_updateAdd = 0;
            curr_v->_set = true;
            curr_v->_updateSet = vertex->_updateSet;
        }
        
        if (vertex->_add) {
            curr_v->_add = true;
            curr_v->_updateAdd += vertex->_updateAdd;
        }
        curr_v->_reverse ^= vertex->_reverse;
    };
    
    // Push into childs
    PushHelp(vertex->left);
    PushHelp(vertex->right);
    
    
    vertex->_add = vertex->_set = vertex->_reverse = false;
    vertex->_updateSet = vertex->_updateAdd = 0;
}

void SplayTree::_Rotate(Node* target, bool is_left)
{
    if (target == nullptr || target->parent == nullptr) {
        return;
    }
    
    Node* local_root = target->parent;
    
    Node::Push(local_root);
    Node::Push(target);
    
    local_root->SetChild((is_left ? target->left : target->right), (is_left ? Node::Right : Node::Left));
    
    _Transplant(local_root, target);
    
    target->SetChild(local_root, (is_left ? Node::Left : Node::Right));
    
    Node::Update(local_root);
    Node::Update(target);
}

void SplayTree::_RightRotate(Node* target)
{
    _Rotate(target, false);
}

void SplayTree::_LeftRotate(Node* target)
{
    _Rotate(target, true);
}

void SplayTree::_ZigMove(Node* pivot)
{
    if (pivot == nullptr || pivot->parent == nullptr) {
        return;
    }
    if (pivot->parent->left == pivot) {
        _RightRotate(pivot);
    } else {
        _LeftRotate(pivot);
    }
}

void SplayTree::_ZigZigMove(Node* pivot)
{
    if (pivot == nullptr || pivot->parent == nullptr || pivot->parent->parent == nullptr) {
        return;
    }
    
    if (pivot->parent->left == pivot) {
        _RightRotate(pivot->parent);
        _RightRotate(pivot);
    } else {
        _LeftRotate(pivot->parent);
        _LeftRotate(pivot);
    }
}

void SplayTree::_ZigZagMove(Node* pivot)
{
    if (pivot == nullptr || pivot->parent == nullptr || pivot->parent->parent == nullptr) {
        return;
    }
    
    if (pivot->parent->left == pivot) {
        _RightRotate(pivot);
        _LeftRotate(pivot);
    } else {
        _LeftRotate(pivot);
        _RightRotate(pivot);
    }
}

void SplayTree::_Splay(Node* pivot)
{
    if (pivot == nullptr) {
        return;
    }
    while (pivot->parent != nullptr) {
        if (pivot->parent->parent == nullptr) {
            _ZigMove(pivot);
        } else {
            Node* pivot_parent = pivot->parent;
            if (pivot_parent->left == pivot) {
                if (pivot_parent->parent->left == pivot_parent) {
                    _ZigZigMove(pivot);
                } else {
                    _ZigZagMove(pivot);
                }
            } else {
                if (pivot_parent->parent->right == pivot_parent) {
                    _ZigZigMove(pivot);
                } else {
                    _ZigZagMove(pivot);
                }
            }
        }
    }
}

void SplayTree::_Transplant(Node* local_root, Node* local_child)
{
    if (local_root == nullptr) {
        return;
    }
    if (local_root->parent == nullptr) {
        _root = local_child;
    } else {
        Node* local_parent = local_root->parent;
        
        if (local_parent->left == local_root) {
            local_parent->left = local_child;
        } else {
            local_parent->right = local_child;
        }
    }
    
    if (local_child != nullptr) {
        local_child->parent = local_root->parent;
    }
}

SplayTree::Node* SplayTree::_FindVertex(Node* vertex, size_t pos)
{
    if (vertex == nullptr) {
        return nullptr;
    }
    while (vertex->left != nullptr || vertex->right != nullptr) {
        
        Node::Push(vertex);
        if (pos == Node::GetField<size_t>(vertex->left, &Node::size, 0u)) {
            break;
        }
        if (pos < Node::GetField<size_t>(vertex->left, &Node::size, 0u)) {
            if (vertex->left == nullptr) {
                break;
            }
            vertex = vertex->left;
        } else {
            if (vertex->right == nullptr) {
                break;
            }
            pos -= Node::GetField<size_t>(vertex->left, &Node::size, 0u) + 1;
            vertex = vertex->right;
        }
    }
    
    _Splay(vertex);
    
    return vertex;
}

SplayTree::Node* SplayTree::_Merge(Node* lhs, Node* rhs)
{
    if (lhs == nullptr) {
        return rhs;
    }
    if (rhs == nullptr) {
        return lhs;
    }
    
    lhs = _FindVertex(lhs, Node::GetField<size_t>(lhs, &Node::size, 0u));
    lhs->SetChild(rhs, Node::Right);
    
    Node::Update(rhs);
    Node::Update(lhs);
    
    return lhs;
}

std::pair<SplayTree::Node*, SplayTree::Node*> SplayTree::_Split(Node* local_root, size_t indx)
{
    if (local_root == nullptr) {
        return {nullptr, nullptr};
    }
    
    if (Node::GetField<size_t>(local_root, &Node::size, 0u) <= indx) {
        return {local_root, nullptr};
    }
    
    local_root = _FindVertex(local_root, indx);
    
    Node* left_part = local_root->left;
    local_root->left = nullptr;
    
    if (left_part != nullptr) {
        left_part->parent = nullptr;
    }
    
    Node::Push(left_part);
    Node::Update(left_part);
    Node::Update(local_root);
    
    return {left_part, local_root};
}

void SplayTree::Delete(size_t pos)
{
    if (_root == nullptr) {
        return;
    }
    
    Node* vertex_to_del = _FindVertex(_root, pos);
    
    Node* left_part = vertex_to_del->left;
    Node* right_part = vertex_to_del->right;
    
    vertex_to_del->left = vertex_to_del->right = nullptr;
    
    if (left_part != nullptr) {
        left_part->parent = nullptr;
    }
    if (right_part != nullptr) {
        right_part->parent = nullptr;
    }
    
    _root = _Merge(left_part, right_part);
    
    delete vertex_to_del;
}


void SplayTree::Insert(const long long& new_elem, size_t pos)
{
    Node* new_node = new Node(new_elem);
    auto it = _Split(_root, pos);
    _root= _Merge(_Merge(it.first, new_node), it.second);
}


template <typename T>
T SplayTree::_SubsegmentOperation(size_t begin, size_t end, std::function<T(Node*&)> func)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    T result = func(it2.second);
    
    _root = _Merge(_Merge(it2.first, it2.second), it.second);
    
    return result;
}

template <>
void SplayTree::_SubsegmentOperation<void>(size_t begin, size_t end, std::function<void(Node*&)> func)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    func(it2.second);
    
    _root = _Merge(_Merge(it2.first, it2.second), it.second);
}

long long SplayTree::GetSummOnSubsegment(size_t begin, size_t end)
{
    return _SubsegmentOperation<long long>(begin, end, [](Node* vertex){ return Node::GetField<long long>(vertex, &Node::summ, 0); });
}

void SplayTree::SetElemOnSubsegment(const long long& elem, size_t begin, size_t end)
{
    return _SubsegmentOperation<void>(begin, end, [&elem](Node* vertex){ return Node::SetFlag_SetElem(vertex, elem); });
}

void SplayTree::AddElemOnSubsegment(const long long& elem, size_t begin, size_t end)
{
    return _SubsegmentOperation<void>(begin, end, [&elem](Node* vertex){ return Node::SetFlag_AddElem(vertex, elem); });
}

void SplayTree::_PermutationOnSubsegment(Node*& local_root, bool next_perm)
{
    std::function<bool(long long, long long)> comp;
    if (next_perm) {
        comp = [](long long lhs, long long rhs) {
            return lhs > rhs;
        };
    } else {
        comp = [](long long lhs, long long rhs) {
            return lhs < rhs;
        };
    }
    
    size_t Suffix = Node::GetField<size_t>(local_root, (next_perm ? &Node::_lowerSuffix : &Node::_upperSuffix), 0);
    size_t left_size = Node::GetField<size_t>(local_root, &Node::size, 0u) - Suffix;
    
    if (left_size == 0) {
        Node::SetFlag_Reverse(local_root);
        return;
    }
    
    auto split1 = _Split(local_root, left_size);
    auto split2 = _Split(split1.first, Node::GetField<size_t>(split1.first, &Node::size, 0u) - 1);
    
    auto local_left_part = split2.first;
    auto vertex_to_swap = split2.second;
    long long element_to_swap = vertex_to_swap->data;
    
    Node* vertex = split1.second;
    
    while (vertex != nullptr) {
        
        Node::Push(vertex);
        Node::Update(vertex);
        
        if (comp(vertex->data, element_to_swap)) {
            if (vertex->right == nullptr || !comp((next_perm ? Node::GetField<long long>(vertex->right, &Node::max, INT_MIN) :
                                                               Node::GetField<long long>(vertex->right, &Node::min, INT_MAX)), element_to_swap)) {
                break;
            }
            vertex = vertex->right;
        } else {
            vertex = vertex->left;
        }
    }
    
    _Splay(vertex);
    
    std::swap(vertex->data, vertex_to_swap->data);
    Node::Update(vertex);
    Node::Update(vertex_to_swap);
    
    Node::SetFlag_Reverse(vertex);
    
    local_root = _Merge(_Merge(local_left_part, vertex_to_swap), vertex);
}

void SplayTree::NextPermutationOnSubsegment(size_t begin, size_t end)
{
    return _SubsegmentOperation<void>(begin, end, [this](Node*& vertex){ return _PermutationOnSubsegment(vertex, true); });
}

void SplayTree::PrevPermutationOnSubsegment(size_t begin, size_t end)
{
    return _SubsegmentOperation<void>(begin, end, [this](Node*& vertex){ return _PermutationOnSubsegment(vertex, false); });
}

void SplayTree::PrintTree(std::ostream& fout) {
    DFS(_root, fout);
}

void solve(std::istream& fin, std::ostream& fout)
{
    SplayTree tree;
    
    size_t n;
    fin >> n;
    
    for (size_t i = 0; i < n; i++) {
        long long elem;
        fin >> elem;
        tree.Insert(elem, i);
    }
    
    size_t q;
    fin >> q;
    
    for (size_t i = 0; i < q; i++) {
        long long type;
        fin >> type;
        
        long long x;
        size_t pos, l, r;
        switch (type) {
            case 1:
                fin >> l >> r;
                
                fout << tree.GetSummOnSubsegment(l, r) << "\n";
                
                break;
                
            case 2:
                fin >> x >> pos;
                
                tree.Insert(x, pos);
                
                break;
                
            case 3:
                fin >> pos;
                
                tree.Delete(pos);
                
                break;
                
            case 4:
                fin >> x >> l >> r;
                
                tree.SetElemOnSubsegment(x, l, r);
                
                break;
                
            case 5:
                fin >> x >> l >> r;
                
                tree.AddElemOnSubsegment(x, l, r);
                
                break;
                
            case 6:
                fin >> l >> r;
                
                tree.NextPermutationOnSubsegment(l, r);
                
                break;
                
            case 7:
                fin >> l >> r;
                   
                tree.PrevPermutationOnSubsegment(l, r);
                   
                break;
        }
    }
    
    tree.PrintTree(fout);
}


int main() {
    using std::cin;
    using std::cout;
    
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    solve(cin, cout);
    
    return 0;
}
