# ifndef IHEAP
# define IHEAP
# include "IHeap.h"
# endif // IHEAP

class BinomialHeap : public IHeap
{
private:
    struct BinomialTree
    {
        int key_, size_;
        BinomialTree *sibling_;
        BinomialTree *largestChild_;

        BinomialTree(const int &key) : key_(key), size_(1), sibling_(nullptr), largestChild_(nullptr) {}
        ~BinomialTree()
        {
            if (sibling_ != nullptr)
                delete sibling_;
            if (largestChild_ != nullptr)
                delete largestChild_;
        }

        void Meld(BinomialTree *other)
        {
            size_ *= 2;
            other->sibling_ = largestChild_;
            largestChild_ = other;
        }
    };

    BinomialTree* GetMinNode() const
    {
        BinomialTree *current = firstNode_;
        int minimal = firstNode_->key_;
        while (current != nullptr)
        {
            minimal = std::min(minimal, current->key_);
            current = current->sibling_;
        }
        current = firstNode_;
        while (current->key_ != minimal)
            current = current->sibling_;
        return current;
    }

    BinomialHeap* HeapFromChildren(BinomialTree *tree)
    {
        if (tree->largestChild_ == nullptr)
            return new BinomialHeap(nullptr);
        BinomialTree *previousChild = nullptr, *currentChild = tree->largestChild_;
        while (currentChild != nullptr)
        {
            BinomialTree *temporal = currentChild->sibling_;
            currentChild->sibling_ = previousChild;
            previousChild = currentChild;
            currentChild = temporal;
        }
        tree->largestChild_ = nullptr;
        return new BinomialHeap(previousChild);
    }

    BinomialTree *firstNode_;
public:
    BinomialHeap() : firstNode_(nullptr) {}

    explicit BinomialHeap(const int &key_)
    {
        firstNode_ = new BinomialTree(key_);
    }

    explicit BinomialHeap(BinomialTree *firstNode) : firstNode_(firstNode) {}

    ~BinomialHeap()
    {
        delete firstNode_;
    }

    bool IsEmpty() const
    {
        return (firstNode_ == nullptr);
    }

    void Meld(IHeap *other)
    {
        BinomialHeap *otherCast = dynamic_cast<BinomialHeap*>(other);
        if (otherCast->firstNode_ == nullptr)
            return;
        BinomialTree *answer;
        BinomialTree *first = firstNode_, *second = otherCast->firstNode_;
        if (firstNode_ == nullptr)
        {
            answer = second;
            second = second->sibling_;
        }
        else
        {
            if (first->size_ < second->size_)
                std::swap(first, second);
            answer = second;
            second = second->sibling_;
        }
        BinomialTree *current = answer;
        while (first != nullptr && second != nullptr)
        {
            if (first->size_ < second->size_)
                std::swap(first, second);
            current->sibling_ = second;
            current = current->sibling_;
            second = second->sibling_;
        }
        if (first == nullptr)
            std::swap(first, second);
        while (first != nullptr)
        {
            current->sibling_ = first;
            current = current->sibling_;
            first = first->sibling_;
        }
        current = answer;
        BinomialTree *previous = nullptr;
        while (current->sibling_ != nullptr)
        {
            BinomialTree *next = current->sibling_;
            if (current->size_ == next->size_)
            {
                if (current->key_ <= next->key_)
                {
                    current->sibling_ = next->sibling_;
                    current->Meld(next);
                }
                else
                {
                    next->Meld(current);
                    if (previous != nullptr)
                        previous->sibling_ = next;
                    else
                        answer = next;
                    current = next;
                }
            }
            else
            {
                previous = current;
                current = current->sibling_;
            }
        }
        firstNode_ = answer;
        otherCast->firstNode_ = nullptr;
    }

    void Insert(const int &key_)
    {
        Meld(dynamic_cast<IHeap*>(new BinomialHeap(key_)));
    }

    int GetMin() const
    {
        int answer = GetMinNode()->key_;
        return answer;
    }

    void ExtractMin()
    {
        BinomialTree *current = GetMinNode(), *previous = nullptr;
        if (current != firstNode_)
        {
            previous = firstNode_;
            while (previous->sibling_ != current)
                previous = previous->sibling_;
        }
        if (previous != nullptr)
            previous->sibling_ = current->sibling_;
        else
        {
            firstNode_ = current->sibling_;
        }
        current->sibling_ = nullptr;
        BinomialHeap *newHeap = HeapFromChildren(current);
        delete current;
        Meld(newHeap);
        delete newHeap;
    }
};
