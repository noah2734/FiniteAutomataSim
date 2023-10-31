#include <iostream>
#include <vector>
#include <string>

struct State {
    std::string name;
    bool isStart;
    bool isAccept;
};

struct Transition {
    State from;
    State to;
    char symbol;
};

struct DFA {
    std::vector<State> states;
    std::vector<char> alphabet;
    std::vector<Transition> transitions;
    State start;
    std::vector<State> accept;

    DFA(std::vector<State> states, std::vector<char> alphabet, std::vector<Transition> transitions) {
        this->states = states;
        this->alphabet = alphabet;
        this->transitions = transitions;
        setStart();
        setAccept();
    }
    void setStart() {
        for (int i = 0; i < states.size(); i++) {
            if (states[i].isStart) {
                start = states[i];
            }
        }
    }

    void setAccept() {
        for (int i = 0; i < states.size(); i++) {
            if (states[i].isAccept) {
                accept.push_back(states[i]);
            }
        }
    }

    void printDFA() {
        std::cout << "States: ";
        std::cout << "{";
        for (int i = 0; i < states.size() - 1; i++) {
            std::cout << states[i].name << ", ";
        }
        std::cout << states[states.size() - 1].name << "}\n";

        std::cout << "Alphabet: ";
        std::cout << "{";
        for (int i = 0; i < alphabet.size() - 1; i++) {
            std::cout << alphabet[i] << ", ";
        }
        std::cout << alphabet[alphabet.size() - 1] << "}\n";

        std::cout << "Transitions:\n";
        for (int i = 0; i < transitions.size(); i++) {
            std::cout << "(" << transitions[i].from.name << ", " << transitions[i].symbol << ")" << " -> " << transitions[i].to.name << "\n";
        }

        std::cout << "Start state: ";
        std::cout << start.name << "\n";

        std::cout << "Accept states: ";
        std::cout << "{";
        for (int i = 0; i < accept.size() - 1; i++) {
            std::cout << accept[i].name << ", ";
        }
        std::cout << accept[accept.size() - 1].name << "}\n";
    }
};

void getStates(std::vector<State> *states) {
    std::cout << "How many states does it have?\n";
    int numStates;
    std::cin >> numStates;
    int k = 0;
    for (int i = 0; i < numStates; i++) {
        std::cout << "What is the name of state " << i << "?\n";
        std::string name;
        std::cin >> name;
        bool start = false;
        if (k == 0) {
            std::cout << "Is state " << i << " the start state? (y/n)\n";
            char isStart;
            std::cin >> isStart;
            if (isStart == 'y') {
                start = true;
                k++;
            }
        }
         
        std::cout << "Is state " << i << " an accept state? (y/n)\n";
        char isAccept;
        std::cin >> isAccept;
        bool accept = false;

        if (isAccept == 'y') {
            accept = true;
        }
        (*states).push_back({name, start, accept});
    }
}

void getAlphabet(std::vector<char> *alphabet) {
     std::cout << "How many symbols are in the alphabet?\n";
    int numSymbols;
    std::cin >> numSymbols;
    
    for (int i = 0; i < numSymbols; i++) {
        std::cout << "What is the " << i << "th symbol?\n";
        char symbol;
        std::cin >> symbol;
        (*alphabet).push_back(symbol);
    }
}

/*void getAcceptStates(std::vector<State> *states) {
    std::cout << "How many accept states are there?\n";
    int numAccept;
    std::cin >> numAccept;
    
    for (int i = 0; i < numAccept; i++) {
        //stop
        for (int j = 0; j < (*states).size(); j++) {
            std::cout << j+1 <<". " << (*states)[j].name << "\n";
        }
        std::cout << "Select state(enter 1-" << (*states).size() << ")\n";
        int stateNum;
        std::cin >> stateNum;
        (*states)[stateNum-1].isAccept = true;
    }
}*/

void getTransitions(std::vector<State> *states, std::vector<char> *alphabet, std::vector<Transition> *transitions) {
    std::cout << "For each state, enter the transitions for each symbol in the alphabet.\n";
    for (int i = 0; i < (*states).size(); i++) {
        for (int j = 0; j < (*alphabet).size(); j++) {
            std::cout << "What is the transition for state " << (*states)[i].name << " on symbol " << (*alphabet)[j] << "?\n";
            std::string toName;
            std::cin >> toName;
            bool found = false;
            for (int k = 0; k < (*states).size(); k++) {
                if ((*states)[k].name == toName) {
                    Transition transition = {(*states)[i], (*states)[k], (*alphabet)[j]};
                    (*transitions).push_back(transition);
                    found = true;
                }
            }
            if (!found) {
                std::cout << "State not found!\n";
                std::cout << "Would you like to enter a valid transition, or restart the process for transitions?\n";
                std::cout << "Enter 'r' to restart, or 'v' to enter a valid transition.\n";
                char choice;
                std::cin >> choice;
                if (choice == 'r') {
                    i = -1;
                    j = (*alphabet).size();
                    std::cout << "For each state, enter the transitions for each symbol in the alphabet.\n";
                    (*transitions).clear();
                }
                else if (choice == 'v') {
                    j--;
                }
            }
        }
    }
}

bool testString(std::string input, DFA *dfa) {
        if (input.length() == 0 && (*dfa).start.isAccept) {
            return true;
        }
        
        bool accepted = false;
        //start at start state
        std::string currentState = (*dfa).start.name;
        for (int i = input.length() - 1; i >= 0; i--) {
            accepted = false;
            std::cout << currentState << " -> ";
            //find corresponding transition, and set current state to the state it transitions to
            //std::cout << (*dfa).transitions.size() << "\n";
            for (int j = 0; j < (*dfa).transitions.size(); j++) {
                if (((*dfa).transitions[j].from.name) == currentState && (*dfa).transitions[j].symbol == input[i]) {
                    //std::cout << (*dfa).transitions[j].to.name << "\n";
                    currentState = ((*dfa).transitions[j].to.name);
                    if (i == 0 && (*dfa).transitions[j].to.isAccept) {
                        accepted = true;
                    }
                    break;
                }
            }
        }
        std::cout << currentState << "\n";
        if (accepted) {
            std::cout << "ACCEPT\n";
        }
        else {
            std::cout << "REJECT\n";
        }
}

int main() {
    std::cout << "Let's create a DFA!\n";

    std::vector<State> states;
    getStates(&states);

    std::vector<char> alphabet;
    getAlphabet(&alphabet);

    std::vector<Transition> transitions;
    getTransitions(&states, &alphabet, &transitions);

    DFA dfa = {states, alphabet, transitions};

    std::cout << "DFA created!\n";
    std::cout << "Here is the DFA you created:\n";
    dfa.printDFA();

    std::cout << "Now lets test some strings!\n";

    while (1) {
    std::cout << "Enter a string to test, or enter 'q' to quit.\n";
        std::cout << "Input will be read from right to left.\n";
        std::string input;
        std::cin >> input;
        testString(input, &dfa);
        if (input == "q") {
            break;
        }
    }

    return 0;
}