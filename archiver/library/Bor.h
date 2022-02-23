#pragma once

#include <vector>
#include <iostream>
#include "PriorQueue.cpp"
#include <queue>

class Bor {
private:
    struct Node {
        int a[2];
        size_t symbol;
        size_t priority;

        Node(size_t prior) : priority(prior) {
            a[0] = a[1] = -1;
        }
        Node(size_t prior, size_t chr) : symbol(chr), priority(prior) {
            a[0] = a[1] = -1;
        }

        bool operator<(const Node& second) {
            return priority < second.priority;
        }
        bool operator>(const Node& second) {
            return priority > second.priority;
        }

        bool IsTerminal() {
            return a[0] == -1 && a[1] == -1;
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
    size_t bor_it_ = 0;

    void BuildCodes(std::vector<bool>& cur_code, int i) {
        if (i == -1) {
            return;
        }
        if (nodes_[i].IsTerminal()) {
            codes_[nodes_[i].symbol] = cur_code;
            return;
        }
        cur_code.push_back(false);
        BuildCodes(cur_code, nodes_[i].a[0]);
        cur_code.pop_back();

        cur_code.push_back(true);
        BuildCodes(cur_code, nodes_[i].a[1]);
        cur_code.pop_back();
    }

    void RecursiveInit(const std::vector<bool>& code, size_t code_index, size_t i, size_t symbol) {
        if (code_index == code.size()) {
            std::cerr << "You cannot be there!" << std::endl;
            throw;
        }

        size_t direction = static_cast<size_t>(code[code_index]);
        if (nodes_[i].a[direction] == -1) {
            nodes_[i].a[direction] = nodes_.size();
            if (code_index == code.size() - 1) {
                nodes_.push_back(Node(0, symbol));
                return;
            } else {
                nodes_.push_back(Node(0));
            }
        }
        RecursiveInit(code, code_index + 1, nodes_[i].a[direction], symbol);
    }

public:

    Bor(const std::vector<std::vector<bool>>& codes) : root_(0), terminal_cnt_(0), codes_(codes) {
        nodes_.push_back(Node(0));
        for (size_t i = 0; i < codes_.size(); ++i) {
            if (codes_[i].empty()) {
                continue;
            }
            RecursiveInit(codes_[i], 0, root_, i);
        }
        bor_it_ = root_;
    }

    Bor(std::vector<size_t> symbol_cnt) : terminal_cnt_(symbol_cnt.size()), codes_(259) {
        if (symbol_cnt.empty()) {
            std::cerr << "Empty symbol_cnt in Bor initializing!" << std::endl;
            throw;
        }
        PriorQueue<QueElem> que;
        for (size_t i = 0; i < symbol_cnt.size(); ++i) {
            if (symbol_cnt[i] == 0) {
                continue;
            }
            nodes_.push_back(Node(symbol_cnt[i], i));
            que.Push(QueElem(nodes_.back(), nodes_.size() - 1));
        }
        if (que.Size() == 1) {
            auto fst = que.Top();
            que.Pop();

            nodes_.push_back(Node(fst.node.priority * 2));
            nodes_.back().a[0] = fst.i;

            que.Push(QueElem(nodes_.back(), nodes_.size() - 1));
        } else {
            while (que.Size() > 1) {
                auto fst = que.Top();
                que.Pop();

                auto snd = que.Top();
                que.Pop();

                nodes_.push_back(Node(fst.node.priority + snd.node.priority));
                nodes_.back().a[0] = fst.i;
                nodes_.back().a[1] = snd.i;

                que.Push(QueElem(nodes_.back(), nodes_.size() - 1));
            }
        }

        root_ = que.Top().i;
    }

    std::pair<bool, size_t> GetCode(bool bit) {
        if (nodes_[bor_it_].a[static_cast<size_t>(bit)] == -1) {
            std::cerr << "Bad way in bor!" << std::endl;
            throw;
        }

        bor_it_ = nodes_[bor_it_].a[static_cast<size_t>(bit)];
        if (nodes_[bor_it_].IsTerminal()) {
            size_t symbol = nodes_[bor_it_].symbol;
            bor_it_ = root_;
            return std::make_pair(true, symbol);
        } else {
            return std::make_pair(false, 0);
        }
    }

    std::vector<std::vector<bool>> GetCodes() {
        std::vector<bool> cur_code;
        BuildCodes(cur_code, root_);
        return codes_;
    }
};