#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>

class Participant{
    private:
        std::string id;
        std::string name;
        std::vector<std::string> roles;
        int workload;

    public:
        Participant(const std::string& id, const std::string& name, const std::vector<std::string>& roles);
            std::string getId() const;
            std::string getName() const;
            std::vector<std::string> getRoles() const;
            int getWorkload() const;

            bool hasRole(const std::string& role) const;

            void incrementWorkload();
            void decrementWorkload();
        


        
};

#endif