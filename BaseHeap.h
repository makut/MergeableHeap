# ifndef IHEAP
# define IHEAP
# include "IHeap.h"
# endif // IHEAP

struct LVertex
{
    int key;
    size_t rk;
    LVertex *left, *right;
    LVertex(const int &key) : key(key), rk(1), left(NULL), right(NULL) {}
    ~LVertex()
    {
        delete left;
        delete right;
    }
};

size_t get_rank(LVertex *v)
{
    if (v == NULL) return 0;
    return v->rk;
}

void update(LVertex *v)
{
    if (v == NULL) return;
    v->rk = std::min(get_rank(v->left), get_rank(v->right)) + 1;
}

struct SVertex
{
    int key;
    SVertex *left, *right;
    SVertex(const int &key) : key(key), left(NULL), right(NULL) {}
    ~SVertex()
    {
        delete left;
        delete right;
    }
};

void merger(LVertex *v)
{
    if (get_rank(v->left) < get_rank(v->right)) std::swap(v->left, v->right);
    update(v);
}

void merger(SVertex *v)
{
    std::swap(v->left, v->right);
}

template<typename Vertex>
class BaseHeap : public IHeap
{
private:
    Vertex* Meld(Vertex *first, Vertex *second)
    {
        if (first == NULL) return second;
        if (second == NULL) return first;
        if (second->key < first->key) std::swap(first, second);
        first->right = Meld(first->right, second);
        merger(first);
        return first;
    }

    Vertex *root;
public:
    BaseHeap() : root(NULL) {}

    explicit BaseHeap(const int &key)
    {
        root = new Vertex(key);
    }

    ~BaseHeap()
    {
        delete root;
    }

    bool IsEmpty() const
    {
        return (root == NULL);
    }

    void Meld(IHeap *Iother)
    {
        BaseHeap *other = dynamic_cast<BaseHeap*>(Iother);
        root = Meld(root, other->root);
    }

    void Insert(const int &key)
    {
        Meld(dynamic_cast<IHeap*>(new BaseHeap(key)));
    }

    int GetMin() const
    {
        return root->key;
    }

    void ExtractMin()
    {
        Vertex *left = root->left, *right = root->right;
        root->left = NULL; root->right = NULL;
        delete root;
        root = Meld(left, right);
    }
};

typedef BaseHeap<LVertex> LHeap;
typedef BaseHeap<SVertex> SHeap;
