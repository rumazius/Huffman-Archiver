#include "Trie.h"

#include "PriorQueue.h"

#include <vector>
#include <iostream>

void Trie::BuildCodes(std::vector<bool>& cur_code, int i) {
    if (i == -1) {
        return;
    }
    if (nodes_[i].IsTerminal()) {
        codes_[nodes_[i].symbol] = cur_code;
        return;
    }
    cur_code.push_back(false);
    BuildCodes(cur_code, nodes_[i].direction[0]);
    cur_code.pop_back();

    cur_code.push_back(true);
    BuildCodes(cur_code, nodes_[i].direction[1]);
    cur_code.pop_back();
}

void Trie::RecursiveInit(const std::vector<bool>& code, size_t code_index, size_t i, size_t symbol) {
    if (code_index == code.size()) {
        std::cerr << "You cannot be there! (Trie RecirsiveInit)" << std::endl;
        throw;
    }

    size_t direction = static_cast<size_t>(code[code_index]);
    if (nodes_[i].direction[direction] == -1) {
        nodes_[i].direction[direction] = nodes_.size();
        if (code_index == code.size() - 1) {
            nodes_.push_back(Node(0, symbol));
            return;
        } else {
            nodes_.push_back(Node(0));
        }
    }

    RecursiveInit(code, code_index + 1, nodes_[i].direction[direction], symbol);
}

Trie::Trie(const std::vector<std::vector<bool>>& codes) : root_(0), terminal_cnt_(0), codes_(codes) {
    nodes_.push_back(Node(0));
    for (size_t i = 0; i < codes_.size(); ++i) {
        if (codes_[i].empty()) {
            continue;
        }
        RecursiveInit(codes_[i], 0, root_, i);
    }
    trie_it_ = root_;
}

std::pair<bool, size_t> Trie::GetCode(bool bit) {
    if (nodes_[trie_it_].direction[static_cast<size_t>(bit)] == -1) {
        std::cerr << "Bad way in bor!" << std::endl;
        throw;
    }

    trie_it_ = nodes_[trie_it_].direction[static_cast<size_t>(bit)];
    if (nodes_[trie_it_].IsTerminal()) {
        size_t symbol = nodes_[trie_it_].symbol;
        trie_it_ = root_;
        return std::make_pair(true, symbol);
    } else {
        return std::make_pair(false, 0);
    }
}

std::vector<std::vector<bool>> Trie::GetCodes() {
    std::vector<bool> cur_code;
    BuildCodes(cur_code, root_);
    return codes_;
}

Trie::Trie(std::vector<size_t> symbol_cnt) : terminal_cnt_(symbol_cnt.size()), codes_(259) {
    if (symbol_cnt.empty()) {
        std::cerr << "Empty symbol_cnt_ in Trie initializing!" << std::endl;
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
        nodes_.back().direction[0] = fst.i;

        que.Push(QueElem(nodes_.back(), nodes_.size() - 1));
    } else {
        while (que.Size() > 1) {
            auto fst = que.Top();
            que.Pop();

            auto snd = que.Top();
            que.Pop();

            nodes_.push_back(Node(fst.node.priority + snd.node.priority));
            nodes_.back().direction[0] = fst.i;
            nodes_.back().direction[1] = snd.i;

            que.Push(QueElem(nodes_.back(), nodes_.size() - 1));
        }
    }

    root_ = que.Top().i;
}