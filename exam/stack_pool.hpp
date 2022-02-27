template <typename T, typename N>
class stack_iterator;

template <typename T, typename N = std::size_t>
class stack_pool{
	struct node_t{
		T value;
		N next;

		//Implement default operations for the rule of six
		node_t() = default;
		node_t(const node_t&) = default;
		node_t(node_t&&) noexcept = default;
		node_t& operator=(const node_t&) = default;
		node_t& operator=(node_t&&) noexcept = default;
		~node_t() noexcept = default;

		node_t(T value, N next) {this->value= value; this->next=next;}
	};

	std::vector<node_t> pool;
	using stack_type = N;
	using value_type = T;
	using size_type = typename std::vector<node_t>::size_type;
	stack_type free_nodes; // at the beginning, it is empty

	node_t& node(stack_type x) noexcept { return pool[x-1]; }
	const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

	stack_type new_node(T& value, stack_type next) {
	if (!empty(free_nodes))
	return take_free_node(value, next);
	pool.push_back(node_t(value, next));
	return pool.size();
	}
	stack_type push_free_nodes (stack_type x) noexcept {
	node(x).next = free_nodes;
	return x;
	}
	stack_type take_free_node (T& value, stack_type next) noexcept {
	stack_type idx{free_nodes};
	free_nodes = node(free_nodes).next;
	node(idx).value = value;
	node(idx).next = next;
	return idx;
	}

public:
	//Implement default operations for the rule of six
	stack_pool() = default;
	stack_pool(const stack_pool&) = default;
	stack_pool(stack_pool&&) noexcept = default;
	stack_pool& operator=(const stack_pool&) = default;
	stack_pool& operator=(stack_pool&&) noexcept = default;
	~stack_pool() noexcept = default;

	explicit stack_pool(size_type n) {pool = std::vector<node_t>(); pool.reserve(n); free_nodes = end();} // reserve n nodes in the pool

	using iterator = stack_iterator<T, N>;
	using const_iterator = stack_iterator<const T, const N>;

	iterator begin(stack_type x) {return iterator(this, x);}
	iterator end(stack_type ) {return iterator(this, end());} //returning the end value for every stack

	const_iterator begin(stack_type x) const {return iterator(this, x);}
	const_iterator end(stack_type ) const {return iterator(this, end());}

	const_iterator cbegin(stack_type x) const {return iterator(this, x);}
	const_iterator cend(stack_type ) const {return iterator(this, end());}

	stack_type new_stack() noexcept {return end();} // return an empty stack

	void reserve(size_type n) {pool.reserve(n);} // reserve n nodes in the pool
	size_type capacity() const noexcept {return pool.capacity();} // the capacity of the pool

	bool empty(stack_type x) const noexcept {return x == end();}

	stack_type end() const noexcept { return stack_type(0); }

	T& value(stack_type x) noexcept {return node(x).value;}
	const T& value(stack_type x) const noexcept {return node(x).value;}

	stack_type& next(stack_type x) noexcept {return node(x).next;}
	const stack_type& next(stack_type x) const noexcept {return node(x).next;}

	stack_type push(const T& val, stack_type head) {
	stack_type new_head{new_node(val, head)};
	return new_head;
	}
	stack_type push(T&& val, stack_type head) {
	stack_type new_head{new_node(val, head)};
	return new_head;
	}


	stack_type pop(stack_type x) noexcept {
	stack_type new_head{node(x).next};
	free_nodes = push_free_nodes(x);
	return new_head;
	} // delete first node

	stack_type free_stack(stack_type x) noexcept {
	if (empty(x))
	return x;
	stack_type tmp = x;
	while(node(tmp).next != end())
	tmp = node(tmp).next;
	node(tmp).next = free_nodes;
	free_nodes = x;
	return new_stack();
	}   // free entire stack
};

template<typename T, typename N>
class stack_iterator {
	using value_type = T;
	using stack_type = N;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	stack_pool<T, N>* pool;
	stack_type idx;
public:
	stack_iterator(stack_pool<T, N>* pool, stack_type x) {this->pool = pool;  idx=x;}

	value_type& operator*() noexcept {return pool->value(idx);}
	const value_type& operator*() const noexcept {return pool->value(idx);}

	stack_iterator operator++() { idx = pool->next(idx); return *this;}
	stack_iterator operator++(int) {stack_iterator tmp = *this; ++(*this); return tmp;}

	friend bool operator== (const stack_iterator& a, const stack_iterator& b) noexcept {return ((a.idx == b.idx) && (a.pool == b.pool));}
	friend bool operator!= (const stack_iterator& a, const stack_iterator& b) noexcept {return ((a.idx != b.idx) || (a.pool != b.pool));}
};

