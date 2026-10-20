#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>

class Participant{
    private:
        int id;
        std::string name;
        std::string role;
        int workload

    public:
        Participant(int id, const std::string& name, const std::string& role);
            int getId() const;
            std::string getName() const;
            std::string getRole() const;
            int getWorkload() const;

            void incrementWorkload();
            void decrementWorkload();
        


        
};

#endif