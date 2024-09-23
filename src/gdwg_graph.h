#ifndef GDWG_GRAPH_H
#	define GDWG_GRAPH_H

#	include <algorithm>
#	include <memory>
#	include <optional>
#	include <set>
#	include <map>
#	include <sstream>
#	include <utility>
#	include <vector>

namespace gdwg {
	// Declaration of graph for class edge uses.
	template<typename N, typename E>
	class graph;

	template<typename N, typename E>
	class edge {
	 public:
		/**
		 * @brief Prints the edge information as a string.
		 * @note Marked as [[nodiscard]] because the string representation of the edge is important and should not be
		 * ignored.
		 *
		 * @return A string representing the edge.
		 */
		[[nodiscard]] virtual auto print_edge() const -> std::string = 0;

		/**
		 * @brief Checks if the edge is weighted.
		 * @note Marked as [[nodiscard]] because this information is critical for distinguishing between weighted and
		 * unweighted edges. Marked as noexcept because it only returns a boolean value without throwing exceptions.
		 *
		 * @return True if the edge is weighted, otherwise false.
		 */
		[[nodiscard]] virtual auto is_weighted() const noexcept -> bool;

		/**
		 * @brief Gets the weight of the edge.
		 * @note Marked as [[nodiscard]] because the weight is important and should not be ignored.
		 * Marked as noexcept because it only accesses a member variable without throwing exceptions.
		 *
		 * @return An optional containing the weight if the edge is weighted, otherwise std::nullopt.
		 */
		[[nodiscard]] virtual auto get_weight() const noexcept -> std::optional<E>;

		/**
		 * @brief Gets the nodes of the edge.
		 * @note Marked as [[nodiscard]] because the nodes are critical for edge identification.
		 * Marked as noexcept because std::make_pair not throw exceptions.
		 *
		 * @return A pair of nodes representing the edge's source and destination.
		 */
		[[nodiscard]] virtual auto get_nodes() const noexcept -> std::pair<N, N>;

		/**
		 * @brief Checks if two edges are equal.
		 * @note Marked as noexcept because it only compares member variables without throwing exceptions.
		 *
		 * @param other The edge to compare with.
		 * @return True if the edges are equal, otherwise false.
		 */
		[[nodiscard]] virtual auto operator==(edge const& other) const noexcept -> bool;

		/**
		 * @brief Destructor.
		 * @note Defaulted.
		 */
		virtual ~edge() = default;

	 protected:
		N src_;
		N dst_;
		std::optional<E> weight_;

		friend class graph<N, E>;

		/**
		 * Default constructor.
		 */
		edge() = default;

		/**
		 * Copy and move constructor in default.
		 */
		edge(edge const&) = default;
		edge(edge&&) noexcept = default;

		/**
		 * Copy and move assignment in default.
		 * @return The instance after copy or move.
		 */
		auto operator=(edge const&) -> edge& = default;
		auto operator=(edge&&) noexcept -> edge& = default;

		/**
		 * @brief Constructs an edge with the given source, destination, and weight.
		 * @note Marked as noexcept because it only performs basic pointer assignments and does not perform any
		 * operations that could throw exceptions.
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 * @param weight The weight of the edge, optional.
		 */
		edge(N const& src, N const& dst, std::optional<E> const& weight = std::nullopt) noexcept;

		/**
		 * @brief Clones the edge, returning a unique pointer to the new edge.
		 * @note Marked as [[nodiscard]] because the cloned pointer is important and should not be ignored.
		 *
		 * @return A unique pointer to the cloned edge.
		 */
		[[nodiscard]] virtual auto clone_ptr() const -> std::unique_ptr<edge> = 0;

		/**
		 * @brief Sets the source and destination nodes of the edge.
		 * @note Marked as noexcept because it only performs basic pointer assignments and does not perform any
		 * operations that could throw exceptions.
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 */
		virtual auto set_nodes(N const& src, N const& dst) noexcept -> void;

		/**
		 * @brief Compares two edges using the three-way comparison operator.
		 * @note Marked as noexcept because it only performs comparisons and does not perform any operations that could
		 * throw exceptions.
		 *
		 * @param other The other edge to compare.
		 * @return A std::strong_ordering result of the comparison.
		 */
		virtual auto edge_comp(edge const& other) const noexcept -> std::strong_ordering;
	};

	template<typename N, typename E>
	class weighted_edge final : public edge<N, E> {
	 public:
		/**
		 * @brief Prints the weighted edge information as a string.
		 * @note Marked as [[nodiscard]] because the string representation of the edge is important and should not be
		 * ignored. Not marked as noexcept because constructing a std::string may throw exceptions.
		 *
		 * @return A string representing the weighted edge.
		 */
		[[nodiscard]] auto print_edge() const -> std::string override;

		/**
		 * @brief Constructs a weighted edge with the given source, destination, and weight.
		 * @note Marked as noexcept because it only performs basic pointer assignments and does not perform any
		 * operations that could throw exceptions.
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 * @param weight The weight of the edge, optional.
		 */
		weighted_edge(N const& src, N const& dst, E const& weight) noexcept;

	 private:
		/**
		 * @brief Clones the weighted edge, returning a unique pointer to the new edge.
		 * @note Marked as [[nodiscard]] because the cloned pointer is important and should not be ignored.
		 * Not marked as noexcept because std::make_unique may throw exceptions.
		 *
		 * @return A unique pointer to the cloned weighted edge.
		 */
		[[nodiscard]] auto clone_ptr() const -> std::unique_ptr<edge<N, E>> override;
		friend class graph<N, E>;
	};

	template<typename N, typename E>
	class unweighted_edge final : public edge<N, E> {
	 public:
		/**
		 * @brief Prints the unweighted edge information as a string.
		 * @note Marked as [[nodiscard]] because the string representation of the edge is important and should not be
		 * ignored. Not marked as noexcept because constructing a std::string may throw exceptions.
		 *
		 * @return A string representing the unweighted edge.
		 */
		[[nodiscard]] auto print_edge() const -> std::string override;

		/**
		 * @brief Constructs an unweighted edge with the given source and destination.
		 * @note Marked as noexcept because it not throw exceptions.
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 */
		unweighted_edge(N const& src, N const& dst) noexcept;

	 private:
		/**
		 * @brief Clones the unweighted edge, returning a unique pointer to the new edge.
		 * @note Marked as [[nodiscard]] because the cloned pointer is important and should not be ignored.
		 * Not marked as noexcept because std::make_unique may throw exceptions.
		 *
		 * @return A unique pointer to the cloned unweighted edge.
		 */
		[[nodiscard]] auto clone_ptr() const -> std::unique_ptr<edge<N, E>> override;
		friend class graph<N, E>;
	};

	template<typename N, typename E>
	class graph {
		/**
		 * The private part using for the member which are store in the graph class.
		 * Using to short name of the long type name, and comparator for the set use to sort and find.
		 * And the private iterator used only for the class graph.
		 */
		struct compare_shared_ptr {
			using is_transparent = std::true_type;

			/**
			 * @brief Compares two shared_ptr<N> instances lexicographically.
			 * @note This function is marked as noexcept because it doesn't perform any operations that could throw
			 * exceptions. Dereferencing a shared_ptr is a safe operation that doesn't throw exceptions.
			 *
			 * @param lhs The left-hand side shared_ptr<N> for comparison.
			 * @param rhs The right-hand side shared_ptr<N> for comparison.
			 * @return A boolean result of the comparison.
			 */
			auto operator()(std::shared_ptr<N> const& lhs, std::shared_ptr<N> const& rhs) const noexcept -> bool;

			/**
			 * @brief Compares a shared_ptr<N> instance with an N instance lexicographically.
			 * @note This function is marked as noexcept because it doesn't perform any operations that could throw
			 * exceptions. Dereferencing a shared_ptr is a safe operation that doesn't throw exceptions.
			 *
			 * @param lhs The left-hand side shared_ptr<N> for comparison.
			 * @param rhs The right-hand side N for comparison.
			 * @return A boolean result of the comparison.
			 */
			auto operator()(std::shared_ptr<N> const& lhs, N const& rhs) const noexcept -> bool;

			/**
			 * @brief Compares an N instance with a shared_ptr<N> instance lexicographically.
			 * @note This function is marked as noexcept because it doesn't perform any operations that could throw
			 * exceptions. Dereferencing a shared_ptr is a safe operation that doesn't throw exceptions.
			 *
			 * @param lhs The left-hand side N for comparison.
			 * @param rhs The right-hand side shared_ptr<N> for comparison.
			 * @return A boolean result of the comparison.
			 */
			auto operator()(N const& lhs, std::shared_ptr<N> const& rhs) const noexcept -> bool;
		};

		// Using type for storing edge with src, dst as the tuple in graph class.
		using edge_tuple = std::tuple<std::shared_ptr<N>, std::shared_ptr<N>, std::shared_ptr<edge<N, E>>>;

		struct compare_edges_set {
			using is_transparent = std::true_type;

			/**
			 * @brief Compares two edge_tuple<N, E> instances lexicographically based on their nodes and edges.
			 * @note This function is marked as noexcept because it doesn't perform any operations that could throw
			 * exceptions. Dereferencing a shared_ptr is a safe operation that doesn't throw exceptions.
			 *
			 * @param lhs The left-hand side edge_tuple<N, E> for comparison.
			 * @param rhs The right-hand side edge_tuple<N, E> for comparison.
			 * @return A boolean result of the comparison.
			 */
			auto operator()(edge_tuple const& lhs, edge_tuple const& rhs) const noexcept -> bool;

		 private:
			compare_shared_ptr comp_shared_node_;
		};

		// Member type of graph, define in private for using at some functions.
		using type_nodes_set = std::set<std::shared_ptr<N>, compare_shared_ptr>;
		using edges_set = std::set<edge_tuple, compare_edges_set>;

		class iter {
		 public:
			using value_type = struct {
				N from;
				N to;
				std::optional<E> weight;
			};
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			/**
			 * Iterator default constructor
			 */
			iter() = default;

			/**
			 * @brief Dereferences the iterator to access the current edge.
			 * @note Marked as [[nodiscard]] because the dereferenced value is important and should not be ignored.
			 * Marked as noexcept because it only involves dereferencing smart pointers and copying values, which do not
			 * throw exceptions.
			 *
			 * @return A reference to the current edge value.
			 */
			[[nodiscard]] auto operator*() const noexcept -> reference;

			/**
			 * @brief Advances the iterator to the next position (pre-increment).
			 * @note Marked as noexcept because it only increments the internal iterator, which does not throw
			 * exceptions.
			 *
			 * @return A reference to the incremented iterator.
			 */
			auto operator++() noexcept -> iter&;

			/**
			 * @brief Advances the iterator to the next position (post-increment).
			 * @note Marked as noexcept because it only increments the internal iterator, which does not throw
			 * exceptions.
			 *
			 * @return A copy of the iterator before incrementing.
			 */
			auto operator++(int) noexcept -> iter;

			/**
			 * @brief Moves the iterator to the previous position (pre-decrement).
			 * @note Marked as noexcept because it only decrements the internal iterator, which does not throw
			 * exceptions.
			 *
			 * @return A reference to the decremented iterator.
			 */
			auto operator--() noexcept -> iter&;

			/**
			 * @brief Moves the iterator to the previous position (post-decrement).
			 * @note Marked as noexcept because it only decrements the internal iterator, which does not throw
			 * exceptions.
			 *
			 * @return A copy of the iterator before decrementing.
			 */
			auto operator--(int) noexcept -> iter;

			/**
			 * @brief Compares two iterators for equality.
			 * @note Marked as noexcept because it only compares internal iterators, which do not throw exceptions.
			 *
			 * @param other The iterator to compare with.
			 * @return True if the iterators are equal, otherwise false.
			 */
			auto operator==(iter const& other) const noexcept -> bool;

		 private:
			using set_iter = typename edges_set::iterator;

			set_iter set_it_;
			/**
			 * @brief Constructs an iterator from a set iterator.
			 * @note Marked as noexcept because it only initializes the internal iterator, which does not throw
			 * exceptions.
			 *
			 * @param set_it The set iterator to initialize with.
			 */
			explicit iter(set_iter const& set_it) noexcept;

			/**
			 * @brief Returns the base iterator.
			 * @note Marked as [[nodiscard]] because the base iterator is important and should not be ignored.
			 * Marked as noexcept because it only returns the internal iterator, which does not throw exceptions.
			 *
			 * @return The base set iterator.
			 */
			[[nodiscard]] auto base() const noexcept -> set_iter;

			friend class graph;
		};

	 public:
		using iterator = iter;
		/**
		 * @brief Default constructor. Constructs an empty graph.
		 * @note Marked as noexcept because it only initializes member variables and does not throw exceptions.
		 */
		graph() noexcept;

		/**
		 * @brief Constructs a graph from an initializer list of nodes.
		 * @note Not marked as noexcept because it may perform operations that could throw exceptions.
		 *
		 * @param il The initializer list of nodes to construct the graph with.
		 */
		graph(std::initializer_list<N> const& il);

		/**
		 * @brief Constructs a graph from a range of nodes.
		 * @note Not marked as noexcept because it may perform operations that could throw exceptions by
		 * std::make_shared.
		 *
		 * @tparam InputIt The type of the input iterator.
		 * @param first The beginning of the range of nodes.
		 * @param last The end of the range of nodes.
		 */
		template<typename InputIt>
		graph(InputIt const& first, InputIt const& last);

		/**
		 * @brief Copy constructor. Constructs a graph as a copy of another graph.
		 *
		 * @param other The graph to copy from.
		 */
		graph(graph const& other);

		/**
		 * @brief Move constructor. Constructs a graph by moving another graph.
		 * @note Marked as noexcept because it only exchanges member variables and does not throw exceptions.
		 *
		 * @param other The graph to move from.
		 */
		graph(graph&& other) noexcept;

		/**
		 * @brief Destructor.
		 * @note Defaulted.
		 */
		~graph() = default;

		/**
		 * @brief Copy assignment operator. Assigns the graph from another graph.
		 *
		 * @param other The graph to assign from.
		 * @return A reference to the assigned graph.
		 */
		auto operator=(graph const& other) -> graph&;

		/**
		 * @brief Move assignment operator. Moves the graph from another graph.
		 * @note Marked as noexcept because it only exchanges member variables and does not throw exceptions.
		 *
		 * @param other The graph to move from.
		 * @return A reference to the assigned graph.
		 */
		auto operator=(graph&& other) noexcept -> graph&;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//                             GRAPH MODIFIER FUNCTIONS                                                       //
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 * @brief Inserts a node into the graph.
		 * @note Not marked as noexcept because std::make_shared may throw.
		 *
		 * @param value The value of the node to insert.
		 * @return True if the node was inserted, otherwise false.
		 */
		auto insert_node(N const& value) -> bool;

		/**
		 * @brief Inserts an edge into the graph.
		 * @note Not marked as noexcept because it may throw exceptions if src or dst do not exist.
		 *
		 * @param src The source node of the edge.
		 * @param dst The destination node of the edge.
		 * @param weight The weight of the edge, optional.
		 * @return True if the edge was inserted, otherwise false.
		 */
		auto insert_edge(N const& src, N const& dst, std::optional<E> const& weight = std::nullopt) -> bool;

		/**
		 * @brief Replaces an existing node with a new node.
		 * @note Not marked as noexcept because it may throw exceptions if old_data does not exist.
		 *
		 * @param old_data The existing node to replace.
		 * @param new_data The new node to replace the old one.
		 * @return True if the node was replaced, otherwise false.
		 */
		auto replace_node(N const& old_data, N const& new_data) -> bool;

		/**
		 * @brief Merges an existing node into another node, replacing the old node.
		 * @note Not marked as noexcept because it may throw exceptions if old_data or new_data do not exist.
		 *
		 * @param old_data The existing node to merge.
		 * @param new_data The node to merge the old one into.
		 */
		auto merge_replace_node(N const& old_data, N const& new_data) -> void;

		/**
		 * @brief Erases a node from the graph.
		 * @note Not marked as noexcept because it may throw exceptions if value does not exist.
		 *
		 * @param value The value of the node to erase.
		 * @return True if the node was erased, otherwise false.
		 */
		auto erase_node(N const& value) -> bool;

		/**
		 * @brief Erases an edge from the graph.
		 * @note Not marked as noexcept because it throws an exception if src or dst do not exist.
		 *
		 * Time complexity: O(log n) for error checking, O(e) for searching to erase. Totally O(log(n) + e).
		 *
		 * @param src The source node of the edge.
		 * @param dst The destination node of the edge.
		 * @param weight The weight of the edge, optional.
		 * @return True if the edge was erased, otherwise false.
		 */
		auto erase_edge(N const& src, N const& dst, std::optional<E> const& weight = std::nullopt) -> bool;

		/**
		 * @brief Erases an edge from the graph using an iterator.
		 * @note Marked as noexcept because the erase operation only throws if the compare function throws, which is
		 * noexcept.
		 *
		 * Time complexity: O(log(e)) for removing an element in a set.
		 *
		 * @param i The iterator pointing to the edge to erase.
		 * @return An iterator to the element following the erased one.
		 */
		auto erase_edge(iterator const& i) noexcept -> iterator;

		/**
		 * @brief Erases a range of edges from the graph.
		 * @note Marked as noexcept because the erase operation only throws if the compare function throws, which is
		 * noexcept.
		 *
		 * Time complexity: O(log(e) + d) for removing a range of elements in a set.
		 *
		 * @param i The iterator pointing to the first edge to erase.
		 * @param s The iterator pointing to the element past the last edge to erase.
		 * @return An iterator to the element following the last erased one.
		 */
		auto erase_edge(iterator const& i, iterator const& s) noexcept -> iterator;

		/**
		 * @brief Clears all nodes and edges from the graph.
		 * @note Marked as noexcept because it only clears the containers, which do not throw exceptions.
		 *
		 * @return void.
		 */
		auto clear() noexcept -> void;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//                            GRAPH ACCESSORS FUNCTIONS                                                       //
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 * @brief Checks if a node exists in the graph.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 * Marked as noexcept because it only checks for the existence of a node in a set, which does not throw
		 * exceptions.
		 *
		 * Time complexity: O(log n) for finding element in a set.
		 *
		 * @param value The value of the node to check.
		 * @return True if the node exists, otherwise false.
		 */
		[[nodiscard]] auto is_node(N const& value) const noexcept -> bool;

		/**
		 * @brief Checks if the graph is empty.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 * Marked as noexcept because it only checks if the set of nodes is empty, which does not throw exceptions.
		 *
		 * @return True if the graph is empty, otherwise false.
		 */
		[[nodiscard]] auto empty() const noexcept -> bool;

		/**
		 * @brief Checks if two nodes are connected by an edge.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 * Not marked as noexcept because it throws an exception if src or dst do not exist.
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 * @return True if the nodes are connected, otherwise false.
		 */
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool;

		/**
		 * @brief Returns a vector of all nodes in the graph.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 *
		 * Time complexity: O(n) for all nodes insertion.
		 *
		 * @return A vector of all nodes in the graph.
		 */
		[[nodiscard]] auto nodes() const -> std::vector<N>;

		/**
		 * @brief Returns a vector of all edges between two nodes.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 * Not marked as noexcept because it throws an exception if src or dst do not exist.
		 *
		 * Time complexity: O(log n) for error checking, O(e) for insertion of edges. Totally O(log n + e).
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 * @return A vector of unique pointers to all edges between the two nodes.
		 */
		[[nodiscard]] auto edges(N const& src, N const& dst) const -> std::vector<std::unique_ptr<edge<N, E>>>;

		/**
		 * @brief Finds an edge in the graph.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 * Marked as noexcept because it only performs search operations that do not throw exceptions.
		 *
		 * Time complexity: O(e), finding iterator by search over the iterator inside graph class.
		 *
		 * @param src The source node.
		 * @param dst The destination node.
		 * @param weight The weight of the edge, optional.
		 * @return An iterator to the edge if found, otherwise end iterator.
		 */
		[[nodiscard]] auto
		find(N const& src, N const& dst, std::optional<E> const& weight = std::nullopt) const noexcept -> iterator;

		/**
		 * @brief Returns a vector of nodes connected to the given source node.
		 * @note Marked as [[nodiscard]] because the result is important and should not be ignored.
		 * Not marked as noexcept because it throws an exception if src does not exist.
		 *
		 * Time complexity: O(log(n)) for error check. O(e) for iterating over the graph class to get the dst nodes and
		 * push to vector and O(e) to remove duplicate element. Totally O(log(n)+e).
		 *
		 * @param src The source node.
		 * @return A vector of nodes connected to the source node.
		 */
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N>;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//                               GRAPH ITERATOR ACCESS FUNCTIONS                                              //
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 * @brief Returns an iterator to the beginning of the edges set.
		 * @note Marked as [[nodiscard]] because the iterator is important and should not be ignored.
		 * Marked as noexcept because it only returns an iterator to the beginning of the set, which does not throw
		 * exceptions.
		 *
		 * @return An iterator to the beginning of the edges set.
		 */
		[[nodiscard]] auto begin() const noexcept -> iterator;

		/**
		 * @brief Returns an iterator to the end of the edges set.
		 * @note Marked as [[nodiscard]] because the iterator is important and should not be ignored.
		 * Marked as noexcept because it only returns an iterator to the end of the set, which does not throw
		 * exceptions.
		 *
		 * @return An iterator to the end of the edges set.
		 */
		[[nodiscard]] auto end() const noexcept -> iterator;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//                                  GRAPH OTHER FUNCTIONS                                                     //
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 * @brief Compares two graphs for equality.
		 * @note Marked as [[nodiscard]] because the result of the comparison is important and should not be ignored.
		 * Marked as noexcept because it does not perform any operations that can throw exceptions.
		 *
		 * Time complexity: O(n + e) where it will compare all nodes and edges.
		 *
		 * @param other The graph to compare with.
		 * @return True if the graphs are equal, otherwise false.
		 */
		[[nodiscard]] auto operator==(graph const& other) const noexcept -> bool;

		/**
		 * @brief Outputs the graph to the given output stream.
		 * @note Not marked as noexcept because streaming to std::ostream can throw.
		 *
		 * @tparam T The type of the nodes in the graph.
		 * @tparam U The type of the edges in the graph.
		 * @param os The output stream to write to.
		 * @param g The graph to output.
		 * @return The output stream.
		 */
		template<typename T, typename U>
		friend auto operator<<(std::ostream& os, graph<T, U> const& g) -> std::ostream&;

	 private:
		type_nodes_set nodes_;
		edges_set edges_;

		/**
		 * @brief Finds a node in the graph and returns a shared pointer to it.
		 * @note Marked as [[nodiscard]] because the returned pointer is important and should not be ignored.
		 * Marked as noexcept because it only performs a lookup and pointer operations which do not throw exceptions.
		 *
		 * @param value The value of the node to find.
		 * @return A shared pointer to the node if found, otherwise nullptr.
		 */
		[[nodiscard]] auto find_node_ptr(N const& value) const noexcept -> std::shared_ptr<N>;

		/**
		 * @brief Updates the source or destination of an edge to a new node.
		 * @note Marked as noexcept because it does not perform any operations that can throw exceptions.
		 *
		 * @param src_ptr The shared pointer to the old node.
		 * @param dst_ptr The shared pointer to the new node.
		 * @return void
		 */
		auto update_node(std::shared_ptr<N> const& src_ptr, std::shared_ptr<N> const& dst_ptr) noexcept -> void;

		/**
		 * @brief Performs a deep copy of the given graph into the current graph instance.
		 *
		 * This method will copy all nodes and edges from the provided graph into the
		 * current graph instance, ensuring that the current graph becomes an exact
		 * duplicate of the other graph. Any existing nodes and edges in the current
		 * graph will be replaced.
		 *
		 * @param other The graph to copy from.
		 * @return void
		 */
		auto deep_copy(graph const& other) -> void;
	};
	// Redeclaration of graph friend function operator<<
	template<typename T, typename U>
	auto operator<<(std::ostream& os, graph<T, U> const& g) -> std::ostream&;
} // namespace gdwg

#endif // GDWG_GRAPH_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             COMPARE SHARED PTR FUNCTIONS                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::compare_shared_ptr::operator()(std::shared_ptr<N> const& lhs,
                                                       std::shared_ptr<N> const& rhs) const noexcept -> bool {
	return *lhs < *rhs;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::compare_shared_ptr::operator()(std::shared_ptr<N> const& lhs,
                                                       N const& rhs) const noexcept -> bool {
	return *lhs < rhs;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::compare_shared_ptr::operator()(N const& lhs,
                                                       std::shared_ptr<N> const& rhs) const noexcept -> bool {
	return lhs < *rhs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             COMPARE EDGES SET FUNCTIONS                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::compare_edges_set::operator()(edge_tuple const& lhs,
                                                      edge_tuple const& rhs) const noexcept -> bool {
	// Compare first node
	if (std::get<0>(lhs) != std::get<0>(rhs))
		return comp_shared_node_(std::get<0>(lhs), std::get<0>(rhs));

	// Compare second node
	if (std::get<1>(lhs) != std::get<1>(rhs))
		return comp_shared_node_(std::get<1>(lhs), std::get<1>(rhs));

	// Compare edge
	return std::get<2>(lhs)->edge_comp(*std::get<2>(rhs)) == std::strong_ordering::less;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  EDGE FUNCTIONS                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
gdwg::edge<N, E>::edge(N const& src, N const& dst, std::optional<E> const& weight) noexcept
: src_{src}
, dst_{dst}
, weight_{weight} {}

template<typename N, typename E>
auto gdwg::edge<N, E>::is_weighted() const noexcept -> bool {
	return weight_ != std::nullopt;
}

template<typename N, typename E>
auto gdwg::edge<N, E>::get_weight() const noexcept -> std::optional<E> {
	return weight_;
}

template<typename N, typename E>
auto gdwg::edge<N, E>::get_nodes() const noexcept -> std::pair<N, N> {
	return std::make_pair(src_, dst_);
}

template<typename N, typename E>
auto gdwg::edge<N, E>::operator==(edge const& other) const noexcept -> bool {
	return get_nodes() == other.get_nodes() and get_weight() == other.get_weight();
}

template<typename N, typename E>
auto gdwg::edge<N, E>::set_nodes(N const& src, N const& dst) noexcept -> void {
	src_ = src;
	dst_ = dst;
}

template<typename N, typename E>
auto gdwg::edge<N, E>::edge_comp(edge const& other) const noexcept -> std::strong_ordering {
	// both has no weight, they are equal
	if (not is_weighted() and not other.is_weighted())
		return std::strong_ordering::equal;

	// lhs has no weight, so it comes first
	if (not is_weighted())
		return std::strong_ordering::less;
	// rhs has no weight, so it comes first
	if (not other.is_weighted())
		return std::strong_ordering::greater;

	// Check if the weight type is a floating-point type
	if constexpr (std::is_floating_point_v<E>) {
		auto cmp = weight_ <=> other.weight_;
		if (cmp == std::partial_ordering::equivalent)
			return std::strong_ordering::equal;
		if (cmp == std::partial_ordering::less)
			return std::strong_ordering::less;
		return std::strong_ordering::greater;
	}
	else {
		// For non-floating-point types, directly return the comparison result
		return weight_ <=> other.weight_;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              WEIGHT EDGE FUNCTIONS                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
gdwg::weighted_edge<N, E>::weighted_edge(N const& src, N const& dst, E const& weight) noexcept
: edge<N, E>{src, dst, std::make_optional(weight)} {}

template<typename N, typename E>
auto gdwg::weighted_edge<N, E>::print_edge() const -> std::string {
	auto oss = std::ostringstream{};
	oss << this->src_ << " -> " << this->dst_ << " | W | " << this->weight_.value();
	return oss.str();
}

template<typename N, typename E>
auto gdwg::weighted_edge<N, E>::clone_ptr() const -> std::unique_ptr<edge<N, E>> {
	return std::make_unique<weighted_edge>(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              UNWEIGHT EDGE FUNCTIONS                                                               //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
gdwg::unweighted_edge<N, E>::unweighted_edge(N const& src, N const& dst) noexcept
: edge<N, E>{src, dst} {}

template<typename N, typename E>
auto gdwg::unweighted_edge<N, E>::print_edge() const -> std::string {
	auto oss = std::ostringstream{};
	oss << this->src_ << " -> " << this->dst_ << " | U";
	return oss.str();
}

template<typename N, typename E>
auto gdwg::unweighted_edge<N, E>::clone_ptr() const -> std::unique_ptr<edge<N, E>> {
	return std::make_unique<unweighted_edge>(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   GRAPH FUNCTIONS                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
gdwg::graph<N, E>::graph() noexcept
: nodes_{type_nodes_set{}}
, edges_{edges_set{}} {}

template<typename N, typename E>
gdwg::graph<N, E>::graph(std::initializer_list<N> const& il)
: graph{il.begin(), il.end()} {}

template<typename N, typename E>
template<typename InputIt>
gdwg::graph<N, E>::graph(InputIt const& first, InputIt const& last)
: graph{} {
	std::transform(first, last, std::inserter(nodes_, nodes_.end()), [](auto const& elem) {
		return std::make_shared<N>(elem);
	});
}

template<typename N, typename E>
gdwg::graph<N, E>::graph(graph const& other)
: graph{} {
	deep_copy(other);
}

template<typename N, typename E>
gdwg::graph<N, E>::graph(graph&& other) noexcept
: nodes_(std::exchange(other.nodes_, type_nodes_set{}))
, edges_(std::exchange(other.edges_, edges_set{})) {}

template<typename N, typename E>
auto gdwg::graph<N, E>::operator=(graph const& other) -> graph& {
	if (this != &other) {
		deep_copy(other);
	}
	return *this;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::operator=(graph&& other) noexcept -> graph& {
	if (this != &other) {
		nodes_ = std::exchange(other.nodes_, type_nodes_set{});
		edges_ = std::exchange(other.edges_, edges_set{});
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             GRAPH MODIFIER FUNCTIONS                                                               //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::insert_node(N const& value) -> bool {
	if (nodes_.contains(value))
		return false;
	auto const& new_node = std::make_shared<N>(value);
	nodes_.insert(new_node);
	return true;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::insert_edge(N const& src, N const& dst, std::optional<E> const& weight) -> bool {
	auto const& src_ptr = find_node_ptr(src);
	auto const& dst_ptr = find_node_ptr(dst);
	if (not src_ptr or not dst_ptr) {
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not "
		                         "exist");
	}
	auto edge_ptr = std::shared_ptr<edge<N, E>>{};
	if (weight == std::nullopt) {
		edge_ptr = std::make_shared<unweighted_edge<N, E>>(unweighted_edge<N, E>{src, dst});
	}
	else {
		edge_ptr = std::make_shared<weighted_edge<N, E>>(weighted_edge<N, E>{src, dst, weight.value()});
	}
	auto const& new_edge = edge_tuple{src_ptr, dst_ptr, edge_ptr};
	if (edges_.contains(new_edge))
		return false;
	edges_.insert(new_edge);
	return true;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::replace_node(N const& old_data, N const& new_data) -> bool {
	auto const& src_ptr = find_node_ptr(old_data);
	if (not src_ptr)
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");

	if (is_node(new_data))
		return false;

	if (old_data != new_data) {
		auto const& new_node = std::make_shared<N>(new_data);
		nodes_.erase(src_ptr);
		nodes_.insert(new_node);
		update_node(src_ptr, new_node);
	}
	return true;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::merge_replace_node(N const& old_data, N const& new_data) -> void {
	auto const& src_ptr = find_node_ptr(old_data);
	auto const& new_ptr = find_node_ptr(new_data);
	if (not src_ptr or not new_ptr)
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't "
		                         "exist in the graph");

	if (old_data != new_data) {
		nodes_.erase(src_ptr);
		update_node(src_ptr, new_ptr);
	}
}

template<typename N, typename E>
auto gdwg::graph<N, E>::erase_node(N const& value) -> bool {
	auto const& node_ptr = find_node_ptr(value);
	if (not node_ptr)
		return false;

	nodes_.erase(node_ptr);
	std::erase_if(edges_, [&node_ptr](auto const& e) {
		auto const& [from, to, edge] = e;
		return *from == *node_ptr or *to == *node_ptr;
	});
	return true;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::erase_edge(N const& src, N const& dst, std::optional<E> const& weight) -> bool {
	if (not is_node(src) or not is_node(dst))
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the "
		                         "graph");

	auto const& edge_ptr = std::ranges::find_if(edges_, [&src, &dst, &weight](auto const& e) {
		auto const& [from, to, edge] = e;
		return *from == src and *to == dst and edge->get_weight() == weight;
	});
	if (edge_ptr == edges_.end())
		return false;

	edges_.erase(edge_ptr);
	return true;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::erase_edge(iterator const& i) noexcept -> iterator {
	auto const& it = i.base();
	return iterator{edges_.erase(it)};
}

template<typename N, typename E>
auto gdwg::graph<N, E>::erase_edge(iterator const& i, iterator const& s) noexcept -> iterator {
	auto const& begin = i.base();
	auto const& end = s.base();
	return iterator{edges_.erase(begin, end)};
}

template<typename N, typename E>
auto gdwg::graph<N, E>::clear() noexcept -> void {
	nodes_.clear();
	edges_.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            GRAPH ACCESSORS FUNCTIONS                                                               //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::is_node(N const& value) const noexcept -> bool {
	return nodes_.contains(value);
}

template<typename N, typename E>
auto gdwg::graph<N, E>::empty() const noexcept -> bool {
	return nodes_.empty();
}

template<typename N, typename E>
auto gdwg::graph<N, E>::is_connected(N const& src, N const& dst) const -> bool {
	if (not is_node(src) or not is_node(dst))
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the "
		                         "graph");

	return std::ranges::find_if(edges_,
	                            [&src, &dst](auto const& e) {
		                            auto const& [src_ptr, dst_ptr, edge_ptr] = e;
		                            return *src_ptr == src and *dst_ptr == dst;
	                            })
	       != edges_.end();
}

template<typename N, typename E>
auto gdwg::graph<N, E>::nodes() const -> std::vector<N> {
	auto vec = std::vector<N>{};
	vec.reserve(nodes_.size());

	// Use std::transform to copy elements to vec
	std::ranges::transform(nodes_, std::back_inserter(vec), [](auto const& n) { return *n; });
	return vec;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::edges(N const& src, N const& dst) const -> std::vector<std::unique_ptr<edge<N, E>>> {
	if (not is_node(src) or not is_node(dst))
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph");

	auto vec = std::vector<std::unique_ptr<edge<N, E>>>{};
	for (auto const& e : edges_) {
		auto const& [src_ptr, dst_ptr, edge_ptr] = e;
		if (*src_ptr == src and *dst_ptr == dst)
			vec.push_back(edge_ptr->clone_ptr());
	}
	return vec;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::find(N const& src, N const& dst, std::optional<E> const& weight) const noexcept -> iterator {
	return std::ranges::find_if(*this, [&src, &dst, &weight](auto const& edge) {
		return edge.from == src and edge.to == dst and edge.weight == weight;
	});
}

template<typename N, typename E>
auto gdwg::graph<N, E>::connections(N const& src) const -> std::vector<N> {
	if (not is_node(src))
		throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");

	auto vec = std::vector<N>{};
	for (auto const& e : edges_) {
		auto const& [src_ptr, dst_ptr, edge_ptr] = e;
		if (*src_ptr == src)
			vec.push_back(*dst_ptr);
	}
	// O(e) for remove duplicate elements by STL algorithm unique.
	auto const& last = std::unique(vec.begin(), vec.end());
	vec.erase(last, vec.end());
	return vec;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                               GRAPH ITERATOR ACCESS FUNCTIONS                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::begin() const noexcept -> iterator {
	return iterator{edges_.begin()};
}

template<typename N, typename E>
auto gdwg::graph<N, E>::end() const noexcept -> iterator {
	return iterator{edges_.end()};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  GRAPH COMPARISON FUNCTIONS                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::operator==(graph const& other) const noexcept -> bool {
	if (nodes_.size() != other.nodes_.size() or edges_.size() != other.edges_.size()) {
		return false;
	}
	// Compare nodes using std::equal
	if (not std::ranges::equal(nodes_, other.nodes_, [](auto const& lhs, auto const& rhs) { return *lhs == *rhs; })) {
		return false;
	}
	// Compare edges using std::equal
	if (not std::ranges::equal(edges_, other.edges_, [](auto const& lhs, auto const& rhs) {
		    return *std::get<2>(lhs) == *std::get<2>(rhs);
	    }))
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  GRAPH EXTRATOR FUNCTIONS                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename U>
auto gdwg::operator<<(std::ostream& os, graph<T, U> const& g) -> std::ostream& {
	os << std::string{"\n"};
	for (auto const& node : g.nodes_) {
		os << *node << " (\n";

		for (auto const& [src, dst, edge] : g.edges_) {
			if (*src == *node) {
				os << std::string{"  "} << edge->print_edge() << '\n';
			}
		}

		os << std::string{")\n"};
	}
	return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                               GRAPH PRIVATE HELPER FUNCTIONS                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::find_node_ptr(N const& value) const noexcept -> std::shared_ptr<N> {
	auto const& it = nodes_.find(value);
	return it != nodes_.end() ? *it : nullptr;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::update_node(std::shared_ptr<N> const& src_ptr,
                                    std::shared_ptr<N> const& dst_ptr) noexcept -> void {
	// Create a new set to hold updated edges
	auto updated_edges = edges_set{};
	for (auto it = edges_.begin(); it != edges_.end();) {
		auto const& [src, dest, edge] = *it;
		// If either src or dest equals src_ptr, insert the updated edge
		if (src == src_ptr or dest == src_ptr) {
			// Use ternary operator to simplify the replacement logic for src and dest
			auto const& new_edge = edge_tuple{(src == src_ptr) ? dst_ptr : src, (dest == src_ptr) ? dst_ptr : dest, edge};

			updated_edges.insert(new_edge);
			edge->set_nodes(*std::get<0>(new_edge), *std::get<1>(new_edge));
			it = edges_.erase(it);
		}
		else {
			++it;
		}
	}
	edges_.insert(updated_edges.begin(), updated_edges.end());
}

template<typename N, typename E>
auto gdwg::graph<N, E>::deep_copy(graph const& other) -> void {
	clear();
	// Copy nodes
	for (auto const& n : other.nodes_) {
		nodes_.insert(std::make_shared<N>(*n));
	}
	// Copy edges
	for (auto const& [src, dst, edge] : other.edges_) {
		insert_edge(*src, *dst, edge->get_weight());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  ITERATOR FUNCTIONS                                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename N, typename E>
auto gdwg::graph<N, E>::iter::operator*() const noexcept -> reference {
	auto const& edge = std::get<2>(*set_it_);
	auto const& [src, dst] = edge->get_nodes();
	return {src, dst, edge->get_weight()};
}

template<typename N, typename E>
gdwg::graph<N, E>::iter::iter(set_iter const& set_it) noexcept
: set_it_{set_it} {}

template<typename N, typename E>
auto gdwg::graph<N, E>::iter::operator++() noexcept -> iter& {
	++set_it_;
	return *this;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::iter::operator++(int) noexcept -> iter {
	auto const it = *this;
	++set_it_;
	return it;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::iter::operator--() noexcept -> iter& {
	--set_it_;
	return *this;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::iter::operator--(int) noexcept -> iter {
	auto const it = *this;
	--set_it_;
	return it;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::iter::operator==(iter const& other) const noexcept -> bool {
	return set_it_ == other.set_it_;
}

template<typename N, typename E>
auto gdwg::graph<N, E>::iter::base() const noexcept -> set_iter {
	return set_it_;
}
