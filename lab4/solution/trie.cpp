#include "trie.hpp"

TTrieNode::TTrieNode() {
    linkFail = nullptr;
}

TTrie::TTrie() {
    root = new TTrieNode();
    root->linkFail = root;
    woJoker = 0;
}

void TTrie::Create(const std::vector<std::string> & patterns) {
    int patternNumber = 0;
    int patLen = patterns.size();
    lensPatterns.resize(patLen);
    int i;

    TTrieNode *curNode = root;
    for (i = 0; i < patLen; ++i) {
        if (patterns[i] == "?") {
            if (curNode != root) {
                curNode->out.push_back(patternNumber);
                lensPatterns[patternNumber] = i - patternNumber;
                ++woJoker;
                curNode = root;
            }
            patternNumber = i + 1;
            continue;
        }

        std::map<unsigned long, TTrieNode *>::iterator nodeToInsert = curNode->to.find(std::stoul(patterns[i]));

        if (nodeToInsert != curNode->to.end()) {
            curNode = nodeToInsert->second;
        } else {
            TTrieNode* newNode = new TTrieNode();
            curNode->to[std::stoul(patterns[i])] = newNode;
            curNode = newNode;
        }
    }

    if (curNode != root) {
        curNode->out.push_back(patternNumber);
        lensPatterns[patternNumber] = i - patternNumber;
        ++woJoker;
    }

    CreateLinks();
}

void TTrie::CreateLinks() {
    TTrieNode* curNode = root;
    std::queue<TTrieNode*> queue;
    queue.push(curNode);

    while(!queue.empty()) {
        curNode = queue.front();
        queue.pop();

        std::map<unsigned long, TTrieNode*>::iterator iter;
        for (iter = curNode->to.begin(); iter != curNode->to.end(); ++iter) {
            TTrieNode* child = iter->second;
            TTrieNode* parentFail = curNode->linkFail;
            unsigned long subPattern = iter->first;

            queue.push(child);

            while(true) {
                std::map<unsigned long, TTrieNode*>::iterator existingNode = parentFail->to.find(subPattern);

                if (existingNode != parentFail->to.end()) {
                    child->linkFail = (existingNode->second != child) ? existingNode->second : root;
                    child->out.insert(child->out.end(), child->linkFail->out.begin(), child->linkFail->out.end());
                    break;
                }
            }
        }
    }
}