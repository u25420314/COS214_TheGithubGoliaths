#ifndef TIVIDY_WORKNODEITERATOR_H
#define TIVIDY_WORKNODEITERATOR_H

class WorkNode;

class WorkNodeIterator {
public:
    virtual ~WorkNodeIterator() {}

    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual WorkNode* current() const = 0;
};

#endif
