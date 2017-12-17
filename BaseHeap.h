# ifndef IHEAP
# define IHEAP
# include "IHeap.h"
# endif // IHEAP

struct SkewVertex
{
    int key;
    SkewVertex *left, *right;
    explicit SkewVertex(const int &key) : key(key), left(nullptr), right(nullptr) {}
    ~SkewVertex()
    {
        delete left;
        delete right;
    }
};

struct LeftistVertex : public SkewVertex
{
    size_t rk;
    explicit LeftistVertex(const int &key) : key(key), left(nullptr), right(nullptr), rk(1) {}
    ~LeftistVertex()
    {
        delete left;
        delete right;
    }
};

size_t get_rank(LeftistVertex *v)
{
    if (v == nullptr)
        return 0;
    return v->rk;
}

void update(LeftistVertex *v)
{
    if (v == nullptr)
        return;
    v->rk = std::min(get_rank(v->left), get_rank(v->right)) + 1;
}

void merger(LeftistVertex *v)
{
    if (get_rank(v->left) < get_rank(v->right))
        std::swap(v->left, v->right);
    update(v);
}

void merger(SkewVertex *v)
{
    std::swap(v->left, v->right);
}

template<typename Vertex>
class BaseHeap : public IHeap
{
private:
    Vertex* Meld(Vertex *first, Vertex *second)
    {
        if (first == nullptr) return second;
        if (second == nullptr) return first;
        if (second->key < first->key)
            std::swap(first, second);
        first->right = Meld(first->right, second);
        merger(first);
        return first;
    }

    Vertex *root_;
public:
    BaseHeap() : root_(nullptr) {}

    explicit BaseHeap(const int &key)
    {
        root_ = new Vertex(key);
    }

    ~BaseHeap()
    {
        delete root_;
    }

    bool IsEmpty() const
    {
        return (root_ == nullptr);
    }

    void Meld(IHeap *other)
    {
        BaseHeap *otherCast = dynamic_cast<BaseHeap*>(other);
        root_ = Meld(root_, otherCast->root_);
    }

    void Insert(const int &key)
    {
        Meld(dynamic_cast<IHeap*>(new BaseHeap(key)));
    }

    int GetMin() const
    {
        return root_->key;
    }

    void ExtractMin()
    {
        Vertex *left = root_->left, *right = root_->right;
        root_->left = nullptr; root_->right = nullptr;
        delete root_;
        root_ = Meld(left, right);
    }
};

typedef BaseHeap<LeftistVertex> LeftistHeap;
typedef BaseHeap<SkewVertex> SkewHeap;
