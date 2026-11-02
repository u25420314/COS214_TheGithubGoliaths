#ifndef TIVIDY_WORKNODEITERATOR_H
#define TIVIDY_WORKNODEITERATOR_H

class WorkNode;

/// @brief The iterator for WorkNode
class WorkNodeIterator {
public:
    /// @brief the destructor for WorkNoteIterator
    virtual ~WorkNodeIterator() {}

    /// @brief searches from the root
    virtual void first() = 0;
    /// @brief gets the next WorkNode
    virtual void next() = 0;
    /// @brief checks if its done
    /// @return if its done or not
    virtual bool isDone() const = 0;
    /// @brief gets the current workNode
    /// @return the WorkNode
    virtual WorkNode* current() const = 0;
};

#endif
