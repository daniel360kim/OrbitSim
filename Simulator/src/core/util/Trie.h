/**
 * @file Trie.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * 
 */

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

template <typename T>
struct TrieNode
{
    std::unordered_map<char, std::unique_ptr<TrieNode<T>>> children;
    std::vector<T> data;
    bool isEndOfWord;
};

template <typename T>
class Trie
{
public:
    Trie();
    ~Trie();

    std::vector<T> search(const std::string &key);

    void insert(const std::string &key, const T &data);
private:
    std::unique_ptr<TrieNode<T>> m_Root;

};

template <typename T>
Trie<T>::Trie()
{
    m_Root = std::make_unique<TrieNode<T>>();
}

template <typename T>
Trie<T>::~Trie()
{
}

template <typename T>
std::vector<T> Trie<T>::search(const std::string &key)
{
    std::vector<T> results;

    auto currentNode = m_Root.get();

    for (size_t i = 0; i < key.length(); i++)
    {
        char character = key[i];
        character = std::tolower(character);
        if (currentNode->children.find(character) == currentNode->children.end())
        {
            return results;
        }
        currentNode = currentNode->children[character].get();
    }

    if (currentNode->children.find('\0') != currentNode->children.end())
    {
        results = currentNode->children['\0']->data;
    }

    return results;
}

template <typename T>
void Trie<T>::insert(const std::string &key, const T &data)
{
    auto currentNode = m_Root.get();

    for (size_t i = 0; i < key.length(); i++)
    {
        char character = key[i];
        character = std::tolower(character);
        if (currentNode->children.find(character) == currentNode->children.end())
        {
            currentNode->children[character] = std::make_unique<TrieNode<T>>();
        }
        currentNode = currentNode->children[character].get();

        std::string substring = key.substr(0, i + 1);
        if (currentNode->children.find('\0') == currentNode->children.end())
        {
            currentNode->children['\0'] = std::make_unique<TrieNode<T>>();
        }
        currentNode->children['\0']->data.push_back(data);
    }

    currentNode->data.push_back(data);
    currentNode->isEndOfWord = true;
}

