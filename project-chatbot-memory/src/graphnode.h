#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include <memory>
#include "chatbot.h"

// forward declarations
class GraphEdge;

class GraphNode
{
private:
    // data handles (owned)
    // CHANGE TASK 4: Make changes to GraphEdge
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes

    // data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes 
    // CHANGE TASK 5: make codes changes required for local ChatBot instance on stack
    ChatBot _chatBot;

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);

    // CHANGE TASK 4: Make Changes to GraphEdge
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);

    // CHANGE TASK 5: make codes changes required for local ChatBot instance on stack
    void MoveChatbotHere(ChatBot chatbot);

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */
