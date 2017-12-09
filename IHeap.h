class IHeap
{
public:
    virtual void Insert(const int &key) = 0;
    virtual bool IsEmpty() const = 0;
    virtual int GetMin() const = 0;
    virtual void ExtractMin() = 0;
    virtual void Meld(IHeap *other) = 0;
    virtual ~IHeap() {}
};
