#pragma once
#include <unordered_map>
#include <list>
#include <chrono>

template <typename Key, typename T, typename Hash = std::hash<Key>> class BoundedCache {
public:
    using Map = std::unordered_map<Key, T, Hash>;
    using iterator = typename Map::iterator;
    using const_iterator = typename Map::const_iterator;

    explicit BoundedCache(size_t max_size) : max_size_(max_size) {}

    iterator find(const Key& key) { return map_.find(key); }
    const_iterator find(const Key& key) const { return map_.find(key); }

    iterator end() { return map_.end(); }
    const_iterator end() const { return map_.end(); }

    T& at(const Key& key) { return map_.at(key); }
    const T& at(const Key& key) const { return map_.at(key); }

    T& operator[](const Key& key) {
        auto it = map_.find(key);
        if (it != map_.end())
            return it->second;

        return emplace(key, T{}).first->second;
    }

    template <typename... Args> std::pair<iterator, bool> emplace(const Key& key, Args&&... args) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            return {it, false};
        }

        order_.push_back(key);
        timestamps_[key] = Clock::now();

        auto result = map_.emplace(key, T(std::forward<Args>(args)...));

        evict_if_needed();

        return result;
    }

    iterator insert_or_assign(const Key& key, T value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second = std::move(value);
            return it;
        }

        return emplace(key, std::move(value)).first;
    }

    size_t size() const { return map_.size(); }

private:
    using Clock = std::chrono::steady_clock;

    void evict_if_needed() {
        while (map_.size() > max_size_) {
            const Key& oldest = order_.front();

            map_.erase(oldest);
            timestamps_.erase(oldest);
            order_.pop_front();
        }
    }

private:
    size_t max_size_;
    Map map_;

    std::list<Key> order_;
    std::unordered_map<Key, typename Clock::time_point, Hash> timestamps_;
};
