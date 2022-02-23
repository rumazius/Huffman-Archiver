#pragma once

#include <vector>

class Trie {
private:
    struct Node {
        int direction[2];
        size_t symbol;
        size_t priority;

        Node(size_t prior) : priority(prior) {
            direction[0] = direction[1] = -1;
        }
        Node(size_t prior, size_t chr) : symbol(chr), priority(prior) {
            direction[0] = direction[1] = -1;
        }

        bool operator<(const Node& second) {
            return priority < second.priority;
        }
        bool operator>(const Node& second) {
            return priority > second.priority;
        }

        bool IsTerminal() {
            return direction[0] == -1 && direction[1] == -1;
        }
    };

    struct QueElem {
        Node node;
        size_t i;
        QueElem(Node n, size_t i) : node(n), i(i) {
        }
        bool operator<(const QueElem& second) {
            return node < second.node;
        }
        bool operator>(const QueElem& second) {
            return node > second.node;
        }
    };

    size_t root_ = 0;
    size_t terminal_cnt_;
    std::vector<Node> nodes_;
    std::vector<std::vector<bool>> codes_;
    size_t trie_it_ = 0;

    void BuildCodes(std::vector<bool>& cur_code, int i);

    void RecursiveInit(const std::vector<bool>& code, size_t code_index, size_t i, size_t symbol);

public:
    Trie(const std::vector<std::vector<bool>>& codes);

    Trie(std::vector<size_t> symbol_cnt);

    std::pair<bool, size_t> GetCode(bool bit);

    std::vector<std::vector<bool>> GetCodes();
};