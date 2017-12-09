# include "IHeap.h"

class LHeap : public IHeap
{
private:
    struct Vertex
    {
        int key;
        size_t rk;
        Vertex *left, *right;

        Vertex(const int &key) : key(key), rk(1), left(NULL), right(NULL) {}
        ~Vertex()
        {
            delete left;
            delete right;
        }
    };

    int get_rank(Vertex *v)
    {
        if (v == NULL) return 0;
        return v->rk;
    }

    void update(Vertex *v)
    {
        if (v == NULL) return;
        v->rk = std::min(get_rank(v->left), get_rank(v->right)) + 1;
    }

    Vertex* Meld(Vertex *first, Vertex *second)
    {
        if (first == NULL) return second;
        if (second == NULL) return first;
        if (second->key < first->key) std::swap(first, second);
        first->right = Meld(first->right, second);
        update(first);
        if (get_rank(first->left) > get_rank(first->right)) std::swap(first->left, first->right);
        return first;
    }

    Vertex *root;
public:
    LHeap() : root(NULL) {}

    explicit LHeap(const int &key)
    {
        root = new Vertex(key);
    }

    ~LHeap()
    {
        delete root;
    }

    bool IsEmpty() const
    {
        return (root == NULL);
    }

    void Meld(IHeap *Iother)
    {
        LHeap *other = dynamic_cast<LHeap*>(Iother);
        root = Meld(root, other->root);
    }

    void Insert(const int &key)
    {
        Meld(dynamic_cast<IHeap*>(new LHeap(key)));
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
