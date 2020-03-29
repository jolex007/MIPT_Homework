#include <iostream>
#include <limits.h>

struct SplayTree
{
private:
    struct Node;
    
    Node* _root;
    
    void _RightRotate(Node*);
    void _LeftRotate(Node*);
    void _Splay(Node*);
    void _Transplant(Node*, Node*);
    Node* _FindVertex(Node*, size_t);
    Node* _Merge(Node*, Node*);
    std::pair<Node*, Node*> _Split(Node*, size_t);
    
    inline void _ZigMove(Node*);
    inline void _ZigZigMove(Node*);
    inline void _ZigZagMove(Node*);
    
    static void DFS(Node*);
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
    
    void PrintTree();
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
    
    void SetRight(Node*);
    void SetLeft(Node*);
    
    static void Update(Node*);
    static void Push(Node*);
    static size_t Size(Node*);
    static long long GetMin(Node*);
    static long long GetMax(Node*);
    static long long GetSumm(Node*);
    static long long GetLeftVertex(Node*);
    static long long GetRightVertex(Node*);
    static size_t GetLowerSuffix(Node*);
    static size_t GetUpperSuffix(Node*);
    static size_t GetLowerPrefix(Node*);
    static size_t GetUpperPrefix(Node*);
    
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

void SplayTree::DFS(Node* vertex)
{
    if (vertex == nullptr) {
        return;
    }
    
    Node::Push(vertex);
    Node::Update(vertex);
    
    DFS(vertex->left);
    std::cout << vertex->data << " ";
    DFS(vertex->right);
}

void SplayTree::Node::SetRight(Node* new_right)
{
    right = new_right;
    if (new_right != nullptr) {
        new_right->parent = this;
    }
}

void SplayTree::Node::SetLeft(Node* new_left)
{
    left = new_left;
    if (new_left != nullptr) {
        new_left->parent = this;
    }
}

void SplayTree::Node::Update(Node* vertex)
{
    if (vertex == nullptr) {
        return;
    }
    
    // Push(vertex);                    // TODO: Maybe need to uncomment
    
    Push(vertex->left);
    Push(vertex->right);
    
    vertex->size = Size(vertex->left) + Size(vertex->right) + 1;
    vertex->summ = GetSumm(vertex->left) + GetSumm(vertex->right) + vertex->data;
    vertex->min = std::min(std::min(GetMin(vertex->left), GetMin(vertex->right)), vertex->data);
    vertex->max = std::max(std::max(GetMax(vertex->left), GetMax(vertex->right)), vertex->data);
    
    vertex->_LeftVertex = (vertex->left == nullptr ? vertex->data : GetLeftVertex(vertex->left));
    vertex->_RightVertex = (vertex->right == nullptr ? vertex->data : GetRightVertex(vertex->right));
    
    vertex->_lowerSuffix = GetLowerSuffix(vertex->right);
    if (GetLowerSuffix(vertex->right) == Size(vertex->right)) {
        if (vertex->right == nullptr || vertex->data >= GetLeftVertex(vertex->right)) {
            vertex->_lowerSuffix++;
            if (vertex->left != nullptr && GetRightVertex(vertex->left) >= vertex->data) {
                vertex->_lowerSuffix += GetLowerSuffix(vertex->left);
            }
        }
    }
    
    vertex->_upperSuffix = GetUpperSuffix(vertex->right);
    if (GetUpperSuffix(vertex->right) == Size(vertex->right)) {
        if (vertex->right == nullptr || vertex->data <= GetLeftVertex(vertex->right)) {
            vertex->_upperSuffix++;
            if (vertex->left != nullptr && GetRightVertex(vertex->left) <= vertex->data) {
                vertex->_upperSuffix += GetUpperSuffix(vertex->left);
            }
        }
    }
    
    vertex->_lowerPrefix = GetLowerPrefix(vertex->left);
    if (GetLowerPrefix(vertex->left) == Size(vertex->left)) {
        if (vertex->left == nullptr || GetRightVertex(vertex->left) >= vertex->data) {
            vertex->_lowerPrefix++;
            if (vertex->right != nullptr && vertex->data >= GetLeftVertex(vertex->right)) {
                vertex->_lowerPrefix += GetLowerPrefix(vertex->right);
            }
        }
    }
    
    vertex->_upperPrefix = GetUpperPrefix(vertex->left);
    if (GetUpperPrefix(vertex->left) == Size(vertex->left)) {
        if (vertex->left == nullptr || GetRightVertex(vertex->left) <= vertex->data) {
            vertex->_upperPrefix++;
            if (vertex->right != nullptr && vertex->data <= GetLeftVertex(vertex->right)) {
                vertex->_upperPrefix += GetUpperPrefix(vertex->right);
            }
        }
    }
}

size_t SplayTree::Node::Size(Node* vertex)
{
    return (vertex == nullptr ? 0u : vertex->size);
}

long long SplayTree::Node::GetMin(Node* vertex)
{
    return (vertex == nullptr ? INT_MAX : vertex->min);
}

long long SplayTree::Node::GetMax(Node* vertex)
{
     return (vertex == nullptr ? INT_MIN : vertex->max);
}

long long SplayTree::Node::GetSumm(Node* vertex)
{
     return (vertex == nullptr ? 0 : vertex->summ);
}

long long SplayTree::Node::GetLeftVertex(Node* vertex)
{
     return (vertex == nullptr ? 0 : vertex->_LeftVertex);
}

long long SplayTree::Node::GetRightVertex(Node* vertex)
{
     return (vertex == nullptr ? 0 : vertex->_RightVertex);
}

size_t SplayTree::Node::GetLowerSuffix(Node* vertex)
{
    return (vertex == nullptr ? 0 : vertex->_lowerSuffix);
}

size_t SplayTree::Node::GetUpperSuffix(Node* vertex)
{
    return (vertex == nullptr ? 0 : vertex->_upperSuffix);
}

size_t SplayTree::Node::GetLowerPrefix(Node* vertex)
{
    return (vertex == nullptr ? 0 : vertex->_lowerPrefix);
}

size_t SplayTree::Node::GetUpperPrefix(Node* vertex)
{
    return (vertex == nullptr ? 0 : vertex->_upperPrefix);
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
    
    // Push info to childs
    if (vertex->left != nullptr) {
        Node* v_left = vertex->left;
        
        if (vertex->_set) {
            v_left->_add = false;
            v_left->_updateAdd = 0;
            v_left->_set = true;
            v_left->_updateSet = vertex->_updateSet;
        }
        
        if (vertex->_add) {
            v_left->_add = true;
            v_left->_updateAdd += vertex->_updateAdd;
        }
        v_left->_reverse ^= vertex->_reverse;
    }
    if (vertex->right != nullptr) {
        Node* v_right = vertex->right;
        
        if (vertex->_set) {
            v_right->_add = false;
            v_right->_updateAdd = 0;
            v_right->_set = true;
            v_right->_updateSet = vertex->_updateSet;
        }
        
        if (vertex->_add) {
            v_right->_add = true;
            v_right->_updateAdd += vertex->_updateAdd;
        }
        v_right->_reverse ^= vertex->_reverse;
    }
    
    vertex->_add = vertex->_set = vertex->_reverse = false;
    vertex->_updateSet = vertex->_updateAdd = 0;
}

void SplayTree::_RightRotate(Node* target)
{
    if (target == nullptr || target->parent == nullptr) {
        return;
    }
    
    Node* local_root = target->parent;
    
    Node::Push(local_root);
    Node::Push(target);
    
    local_root->SetLeft(target->right);
    
    _Transplant(local_root, target);
    
    target->SetRight(local_root);
    
    Node::Update(local_root);
    Node::Update(target);
}

void SplayTree::_LeftRotate(Node* target)
{
    if (target == nullptr || target->parent == nullptr) {
        return;
    }
    
    Node* local_root = target->parent;
    
    Node::Push(local_root);
    Node::Push(target);
    
    local_root->SetRight(target->left);
    
    _Transplant(local_root, target);
    
    target->SetLeft(local_root);
    
    Node::Update(local_root);
    Node::Update(target);
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
        if (pos == Node::Size(vertex->left)) {
            break;
        }
        if (pos < Node::Size(vertex->left)) {
            if (vertex->left == nullptr) {
                break;
            }
            vertex = vertex->left;
        } else {
            if (vertex->right == nullptr) {
                break;
            }
            pos -= Node::Size(vertex->left) + 1;
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
    
    lhs = _FindVertex(lhs, Node::Size(lhs));
    lhs->SetRight(rhs);
    
    Node::Update(rhs);
    Node::Update(lhs);
    
    return lhs;
}

std::pair<SplayTree::Node*, SplayTree::Node*> SplayTree::_Split(Node* local_root, size_t indx)
{
    if (local_root == nullptr) {
        return {nullptr, nullptr};
    }
    
    if (Node::Size(local_root) <= indx) {
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

long long SplayTree::GetSummOnSubsegment(size_t begin, size_t end)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    long long answer = Node::GetSumm(it2.second);
    
    _root = _Merge(_Merge(it2.first, it2.second), it.second);
    
    return answer;
}

void SplayTree::SetElemOnSubsegment(const long long& elem, size_t begin, size_t end)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    Node::SetFlag_SetElem(it2.second, elem);
    
    _root = _Merge(_Merge(it2.first, it2.second), it.second);
}

void SplayTree::AddElemOnSubsegment(const long long& elem, size_t begin, size_t end)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    Node::SetFlag_AddElem(it2.second, elem);
    
    _root = _Merge(_Merge(it2.first, it2.second), it.second);
}

void SplayTree::NextPermutationOnSubsegment(size_t begin, size_t end)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    auto left_part = it2.first;
    auto right_part = it.second;
    auto local_root = it2.second;
    
    size_t lowerSuffix = Node::GetLowerSuffix(local_root);
    size_t left_size = Node::Size(local_root) - lowerSuffix;
    
    if (left_size == 0) {
        Node::SetFlag_Reverse(local_root);
        _root = _Merge(_Merge(left_part, local_root), right_part);
        return;
    }
    
    auto split1 = _Split(local_root, left_size);
    auto split2 = _Split(split1.first, Node::Size(split1.first) - 1);
    
    auto local_left_part = split2.first;
    auto local_right_part = split1.second;
    auto vertex_to_swap = split2.second;
    
    long long element_to_swap = vertex_to_swap->data;
    
    Node* vertex = split1.second;
    
    while (vertex != nullptr) {
        
        Node::Push(vertex);
        Node::Update(vertex);
        
        if (vertex->data > element_to_swap) {
            if (vertex->right == nullptr || Node::GetMax(vertex->right) <= element_to_swap) {
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
    _root = _Merge(_Merge(left_part, local_root), right_part);
}

void SplayTree::PrevPermutationOnSubsegment(size_t begin, size_t end)
{
    auto it = _Split(_root, end + 1);
    auto it2 = _Split(it.first, begin);
    
    auto left_part = it2.first;
    auto right_part = it.second;
    auto local_root = it2.second;
    
    size_t upperSuffix = Node::GetUpperSuffix(local_root);
    size_t left_size = Node::Size(local_root) - upperSuffix;
    
    if (left_size == 0) {
        Node::SetFlag_Reverse(local_root);
        _root = _Merge(_Merge(left_part, local_root), right_part);
        return;
    }
    
    auto split1 = _Split(local_root, left_size);
    auto split2 = _Split(split1.first, Node::Size(split1.first) - 1);
    
    auto local_left_part = split2.first;
    auto vertex_to_swap = split2.second;
    long long element_to_swap = vertex_to_swap->data;
    
    Node* vertex = split1.second;
    
    while (vertex != nullptr) {
        
        Node::Push(vertex);
        Node::Update(vertex);
        
        if (vertex->data < element_to_swap) {
            if (vertex->right == nullptr || Node::GetMin(vertex->right) >= element_to_swap) {
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
    _root = _Merge(_Merge(left_part, local_root), right_part);
}

void SplayTree::PrintTree() {
    DFS(_root);
}


using std::cin;
using std::cout;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    SplayTree tree;
    
    size_t n;
    std::cin >> n;
    
    for (size_t i = 0; i < n; i++) {
        long long elem;
        std::cin >> elem;
        tree.Insert(elem, i);
    }
    
    size_t q;
    std::cin >> q;
    
    for (size_t i = 0; i < q; i++) {
        long long type;
        std::cin >> type;
        
        long long x;
        size_t pos, l, r;
        switch (type) {
            case 1:
                std::cin >> l >> r;
                
                std::cout << tree.GetSummOnSubsegment(l, r) << "\n";
                
                break;
                
            case 2:
                std::cin >> x >> pos;
                
                tree.Insert(x, pos);
                
                break;
                
            case 3:
                std::cin >> pos;
                
                tree.Delete(pos);
                
                break;
                
            case 4:
                std::cin >> x >> l >> r;
                
                tree.SetElemOnSubsegment(x, l, r);
                
                break;
                
            case 5:
                std::cin >> x >> l >> r;
                
                tree.AddElemOnSubsegment(x, l, r);
                
                break;
                
            case 6:
                std::cin >> l >> r;
                
                tree.NextPermutationOnSubsegment(l, r);
                
                break;
                
            case 7:
                std::cin >> l >> r;
                   
                tree.PrevPermutationOnSubsegment(l, r);
                   
                break;
        }
    }
    
    tree.PrintTree();
    
    return 0;
}
