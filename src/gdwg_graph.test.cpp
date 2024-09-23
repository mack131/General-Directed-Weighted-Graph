#include "gdwg_graph.h"

#include <catch2/catch.hpp>

TEST_CASE("basic test") {
	auto g = gdwg::graph<int, std::string>{};
	auto constexpr n = 5;
	g.insert_node(n);
	REQUIRE(g.is_node(n));
}

TEST_CASE("Default graph construct", "[constructor]") {
	SECTION("Default constructor") {
		REQUIRE_NOTHROW(gdwg::graph<std::string, double>{});
	}

	SECTION("Default constructor with empty") {
		auto const& g = gdwg::graph<std::string, double>{};
		REQUIRE(g.empty());
	}
}

TEST_CASE("List graph construct", "[constructor]") {
	SECTION("Initializer list constructor") {
		REQUIRE_NOTHROW(gdwg::graph<std::string, int>{"A", "B"});
	}

	SECTION("List constructor with is_node") {
		auto const& g = gdwg::graph<std::string, int>{"A", "B", "C"};
		REQUIRE(g.is_node("A"));
		REQUIRE(g.is_node("B"));
		REQUIRE(g.is_node("C"));
		REQUIRE_FALSE(g.is_node("D"));
	}
}

TEST_CASE("Range Constructor", "[constructor]") {
	SECTION("With Non-Empty Range") {
		auto const& nodes = std::vector<int>{5, 6, 7, 8};
		auto const& g = gdwg::graph<int, double>{nodes.begin(), nodes.end()};
		REQUIRE_FALSE(g.empty());
		REQUIRE(g.is_node(5));
		REQUIRE(g.is_node(6));
		REQUIRE(g.is_node(7));
		REQUIRE(g.is_node(8));
	}

	SECTION("With Empty Range") {
		auto const& nodes = std::vector<int>{};
		auto const& g = gdwg::graph<int, double>{nodes.begin(), nodes.end()};
		REQUIRE(g.empty());
	}
}

TEST_CASE("Graph Move Constructor", "[constructor]") {
	SECTION("With Non-Empty Graph") {
		auto g1 = gdwg::graph<int, double>{10, 20, 30};
		auto const g2 = std::move(g1);
		REQUIRE_FALSE(g2.empty());
		REQUIRE(g2.is_node(10));
		REQUIRE(g2.is_node(20));
		REQUIRE(g2.is_node(30));
		REQUIRE(g1.empty());
	}

	SECTION("With Empty Graph") {
		auto g1 = gdwg::graph<int, double>{};
		auto const g2 = std::move(g1);
		REQUIRE(g2.empty());
		REQUIRE(g1.empty());
	}

	SECTION("Move operation with iterator validator") {
		auto g1 = gdwg::graph<std::string, double>{};
		g1.insert_node("A");
		g1.insert_node("B");
		g1.insert_node("C");
		g1.insert_node("D");

		g1.insert_edge("A", "B");
		g1.insert_edge("A", "B", 3);
		g1.insert_edge("B", "C");
		g1.insert_edge("A", "D", 5);

		auto it_1 = g1.begin();
		it_1 = std::next(it_1, 2);
		auto const [from, to, weight] = *it_1;

		auto g2 = std::move(g1);
		auto it_2 = g2.begin();
		it_2 = std::next(it_2, 2);
		REQUIRE(it_1 == it_2);

		auto const [from_2, to_2, weight_2] = *it_2;
		REQUIRE(from_2 == from);
		REQUIRE(to_2 == to);
		REQUIRE(weight_2 == weight);

		REQUIRE(++it_1 == ++it_2);
		REQUIRE(std::prev(it_1, 3) == g2.begin());
		REQUIRE(std::next(it_1) == g2.end());
	}
}

TEST_CASE("Graph Copy Constructor", "[constructor]") {
	SECTION("With Non-Empty Graph") {
		auto const& g1 = gdwg::graph<int, double>{10, 20, 30};
		auto const g2 = g1;
		REQUIRE_FALSE(g2.empty());
		REQUIRE(g2.is_node(10));
		REQUIRE(g2.is_node(20));
		REQUIRE(g2.is_node(30));
		// g1 should remain unchanged
		REQUIRE_FALSE(g1.empty());
		REQUIRE(g1.is_node(10));
		REQUIRE(g1.is_node(20));
		REQUIRE(g1.is_node(30));
	}

	SECTION("With Empty Graph") {
		auto const& g1 = gdwg::graph<int, double>{};
		auto const g2 = g1;
		REQUIRE(g2.empty());
		REQUIRE(g1.empty());
	}
}

TEST_CASE("Move Assignment", "[operator]") {
	SECTION("Normal Case") {
		auto g1 = gdwg::graph<int, double>{10, 20, 30};
		auto g2 = gdwg::graph<int, double>{};
		g2 = std::move(g1);
		REQUIRE_FALSE(g2.empty());
		REQUIRE(g2.is_node(10));
		REQUIRE(g2.is_node(20));
		REQUIRE(g2.is_node(30));
		REQUIRE(g1.empty());
	}

	SECTION("Self-Assignment") {
		auto g1 = gdwg::graph<int, double>{10, 20, 30};
		auto& ref = g1;
		g1 = std::move(ref);
		REQUIRE_FALSE(g1.empty());
		REQUIRE(g1.is_node(10));
		REQUIRE(g1.is_node(20));
		REQUIRE(g1.is_node(30));
	}

	SECTION("Move operation with iterator validator") {
		auto g1 = gdwg::graph<std::string, double>{};
		g1.insert_node("A");
		g1.insert_node("B");
		g1.insert_node("C");
		g1.insert_node("D");

		g1.insert_edge("A", "B");
		g1.insert_edge("A", "B", 3);
		g1.insert_edge("B", "C");
		g1.insert_edge("A", "D", 5);

		auto it_1 = g1.begin();
		it_1 = std::next(it_1, 2);
		auto const [from, to, weight] = *it_1;

		auto g2 = gdwg::graph<std::string, double>{};
		g2 = std::move(g1);
		auto it_2 = g2.begin();
		it_2 = std::next(it_2, 2);
		REQUIRE(it_1 == it_2);

		auto const [from_2, to_2, weight_2] = *it_2;
		REQUIRE(from_2 == from);
		REQUIRE(to_2 == to);
		REQUIRE(weight_2 == weight);

		REQUIRE(++it_1 == ++it_2);
		REQUIRE(std::prev(it_1, 3) == g2.begin());
		REQUIRE(std::next(it_1) == g2.end());
	}
}

TEST_CASE("Copy Assignment", "[operator]") {
	SECTION("Normal Case") {
		auto const& g1 = gdwg::graph<int, double>{10, 20, 30};
		auto g2 = gdwg::graph<int, double>{};
		g2 = g1;
		REQUIRE_FALSE(g2.empty());
		REQUIRE(g2.is_node(10));
		REQUIRE(g2.is_node(20));
		REQUIRE(g2.is_node(30));
		REQUIRE_FALSE(g1.empty());
		REQUIRE(g1.is_node(10));
		REQUIRE(g1.is_node(20));
		REQUIRE(g1.is_node(30));
	}

	SECTION("Self-Assignment") {
		auto g1 = gdwg::graph<int, double>{10, 20, 30};
		auto& ref = g1;
		// Self-assignment copy should be handled gracefully
		g1 = ref;
		REQUIRE_FALSE(g1.empty());
		REQUIRE(g1.is_node(10));
		REQUIRE(g1.is_node(20));
		REQUIRE(g1.is_node(30));
	}

	SECTION("Assignment of Empty Graph") {
		auto const& g1 = gdwg::graph<int, double>{};
		auto g2 = gdwg::graph<int, double>{1, 2, 3};
		// Assigning an empty graph to a non-empty one
		g2 = g1;
		REQUIRE(g2.empty());
		REQUIRE(g1.empty());
	}
}

TEST_CASE("Check destructor inside graph", "[constructor]") {
	REQUIRE(std::is_destructible<gdwg::graph<std::string, int>>::value);
	REQUIRE(std::is_destructible<gdwg::graph<char, int>>::value);
	REQUIRE(std::is_destructible<gdwg::graph<int, double>>::value);
}

TEST_CASE("Check destructor inside edge", "[constructor]") {
	REQUIRE(std::is_destructible<gdwg::edge<std::string, int>>::value);
	REQUIRE(std::is_destructible<gdwg::edge<char, double>>::value);
	REQUIRE(std::is_destructible<gdwg::edge<int, bool>>::value);
}

TEST_CASE("Weighted Edge Functionality", "[weighted_edge]") {
	auto constexpr src = 1;
	auto constexpr dst = 2;
	auto constexpr weight = 5;

	SECTION("Construct Weighted Edge") {
		// Ensuring no exceptions and object is created
		REQUIRE_NOTHROW(gdwg::weighted_edge<int, int>{src, dst, weight});
	}

	auto const& edge = gdwg::weighted_edge<int, int>{src, dst, weight};

	SECTION("Print Edge") {
		REQUIRE(edge.print_edge() == "1 -> 2 | W | 5");
	}

	SECTION("Is Weighted") {
		REQUIRE(edge.is_weighted());
	}

	SECTION("Get Weight") {
		REQUIRE(edge.get_weight() == weight);
	}

	SECTION("Get Nodes") {
		auto const& [node_src, node_dst] = edge.get_nodes();
		REQUIRE(node_src == 1);
		REQUIRE(node_dst == 2);
	}

	SECTION("Equality of Weighted Edges") {
		auto const& same_edge = gdwg::weighted_edge<int, int>{src, dst, weight};
		auto const& different_edge = gdwg::weighted_edge<int, int>{src, dst, 10};
		auto un_const_edge = gdwg::weighted_edge<int, int>{src, dst, weight};
		REQUIRE(edge == same_edge);
		REQUIRE_FALSE(edge == different_edge);
		REQUIRE(un_const_edge == edge);
		REQUIRE(un_const_edge == same_edge);
		REQUIRE(un_const_edge != different_edge);
	}
}

TEST_CASE("Unweighted Edge Functionality", "[unweighted_edge]") {
	auto constexpr src = 1;
	auto constexpr dst = 2;

	SECTION("Construct Weighted Edge") {
		// Ensuring no exceptions and object is created
		REQUIRE_NOTHROW(gdwg::unweighted_edge<int, int>{src, dst});
	}

	auto const& edge = gdwg::unweighted_edge<int, int>{src, dst};

	SECTION("Print Edge") {
		REQUIRE(edge.print_edge() == "1 -> 2 | U");
	}

	SECTION("Is Weighted") {
		REQUIRE_FALSE(edge.is_weighted());
	}

	SECTION("Get Weight") {
		REQUIRE(edge.get_weight() == std::nullopt);
	}

	SECTION("Get Nodes") {
		auto const& [node_src, node_dst] = edge.get_nodes();
		REQUIRE(node_src == 1);
		REQUIRE(node_dst == 2);
	}

	SECTION("Equality of unweighted edges and different with weight edge even if weight is 0") {
		auto const& same_edge = gdwg::unweighted_edge<int, int>{src, dst};
		auto const& different_edge = gdwg::weighted_edge<int, int>{src, dst, 0};
		auto un_const_edge = gdwg::unweighted_edge<int, int>{src, dst};
		REQUIRE(edge == same_edge);
		REQUIRE_FALSE(edge == different_edge);
		REQUIRE(un_const_edge == edge);
		REQUIRE(un_const_edge == same_edge);
		REQUIRE(un_const_edge != different_edge);
	}
}

TEST_CASE("Graph insert_node operation", "[insert_node]") {
	auto g = gdwg::graph<int, double>{};

	SECTION("Insert into Empty Graph") {
		REQUIRE(g.empty());
		auto const& inserted = g.insert_node(10);
		REQUIRE(inserted);
		REQUIRE(g.is_node(10));
	}

	SECTION("Insert Duplicate Node") {
		g.insert_node(10);
		auto const inserted_again = g.insert_node(10);
		// Should return false as the node already exists
		REQUIRE_FALSE(inserted_again);
		// Node 10 should still exist
		REQUIRE(g.is_node(10));
	}

	SECTION("Insert Multiple Unique Nodes") {
		auto const inserted1 = g.insert_node(20);
		auto const inserted2 = g.insert_node(30);
		auto const inserted3 = g.insert_node(40);
		REQUIRE(inserted1);
		REQUIRE(inserted2);
		REQUIRE(inserted3);
		REQUIRE(g.is_node(20));
		REQUIRE(g.is_node(30));
		REQUIRE(g.is_node(40));
	}

	SECTION("Insert Multiple Nodes With Duplicates") {
		g.insert_node(20);
		auto const inserted1 = g.insert_node(20);
		auto const inserted2 = g.insert_node(30);
		auto const inserted3 = g.insert_node(40);
		REQUIRE_FALSE(inserted1);
		REQUIRE(inserted2);
		REQUIRE(inserted3);
		REQUIRE(g.is_node(20));
		REQUIRE(g.is_node(30));
		REQUIRE(g.is_node(40));
	}

	SECTION("Insert Node of Different Type") {
		auto sg = gdwg::graph<std::string, double>{};
		auto const inserted = sg.insert_node("Hello");
		REQUIRE(inserted);
		REQUIRE(sg.is_node("Hello"));
	}

	SECTION("Insert Node Edge Case Values") {
		auto const inserted_min = g.insert_node(std::numeric_limits<int>::min());
		auto const inserted_max = g.insert_node(std::numeric_limits<int>::max());
		REQUIRE(inserted_min);
		REQUIRE(inserted_max);
		REQUIRE(g.is_node(std::numeric_limits<int>::min()));
		REQUIRE(g.is_node(std::numeric_limits<int>::max()));
	}
}

TEST_CASE("Graph insert_edge operation", "[insert_edge]") {
	auto g = gdwg::graph<int, double>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	SECTION("Insert Edge with Valid Nodes and Weight") {
		auto const inserted = g.insert_edge(1, 2, 3.14);
		REQUIRE(inserted);
		REQUIRE(g.is_connected(1, 2));
	}

	SECTION("Insert Edge with Valid Nodes and No Weight") {
		auto const inserted = g.insert_edge(1, 3);
		REQUIRE(inserted);
		REQUIRE(g.is_connected(1, 3));
	}

	SECTION("Insert Duplicate Edge") {
		g.insert_edge(1, 2, 3.14);
		auto const inserted_again = g.insert_edge(1, 2, 3.14);
		REQUIRE_FALSE(inserted_again);
	}

	SECTION("Insert Edge with Non-Existent Source Node") {
		REQUIRE_THROWS_MATCHES(g.insert_edge(4, 2, 2.5),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                                                "dst node does not exist"));
	}

	SECTION("Insert Edge with Non-Existent Destination Node") {
		REQUIRE_THROWS_MATCHES(g.insert_edge(1, 4, 2.5),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                                                "dst node does not exist"));
	}

	SECTION("Insert Edge between Same Node") {
		auto const inserted = g.insert_edge(2, 2, 1.0);
		REQUIRE(inserted);
		REQUIRE(g.is_connected(2, 2));
	}

	SECTION("Insert Multiple Edges with Different Weights between Same Nodes") {
		auto const inserted1 = g.insert_edge(2, 3, 1.0);
		auto const inserted2 = g.insert_edge(2, 3, 2.0);
		REQUIRE(inserted1);
		REQUIRE(inserted2);
		REQUIRE(g.is_connected(2, 3));
	}

	SECTION("Insert Edge without Specifying Weight") {
		auto const inserted = g.insert_edge(3, 1);
		REQUIRE(inserted);
		REQUIRE(g.is_connected(3, 1));
	}

	SECTION("Insert unweight and weight for same nodes") {
		REQUIRE(g.insert_edge(1, 2));
		REQUIRE(g.insert_edge(1, 2, 1));
		// Cannot insert again for same weight
		REQUIRE_FALSE(g.insert_edge(1, 2));
		REQUIRE_FALSE(g.insert_edge(1, 2, 1));
	}
}

TEST_CASE("Graph nodes operation", "[nodes]") {
	auto g = gdwg::graph<int, double>{};

	SECTION("Empty Graph") {
		auto const& result = g.nodes();
		// Ensure the nodes list is empty when the graph is empty
		REQUIRE(result.empty());
	}

	SECTION("Graph with Multiple Nodes") {
		g.insert_node(3);
		g.insert_node(1);
		g.insert_node(2);
		auto const& result = g.nodes();
		REQUIRE(result.size() == 3);
		REQUIRE(result == std::vector<int>{1, 2, 3});
	}

	SECTION("Graph with Nodes Inserted in Descending Order") {
		g.insert_node(10);
		g.insert_node(9);
		g.insert_node(8);
		auto const& result = g.nodes();
		REQUIRE(result.size() == 3);
		REQUIRE(result == std::vector<int>{8, 9, 10});
	}

	SECTION("Graph with Negative and Positive Values") {
		g.insert_node(-10);
		g.insert_node(0);
		g.insert_node(10);
		auto const& result = g.nodes();
		REQUIRE(result.size() == 3);
		REQUIRE(result == std::vector<int>{-10, 0, 10});
	}

	SECTION("Graph with Max and Min Integer Values") {
		g.insert_node(std::numeric_limits<int>::max());
		g.insert_node(std::numeric_limits<int>::min());
		auto const& result = g.nodes();
		REQUIRE(result.size() == 2);
		// Ensure edge integers are handled correctly
		REQUIRE(result == std::vector<int>{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()});
	}

	SECTION("Repeated Queries") {
		g.insert_node(1);
		g.insert_node(2);
		// Query multiple times to see if the internal state changes or if performance degrades
		auto const& result1 = g.nodes();
		auto const& result2 = g.nodes();
		REQUIRE(result1 == result2);
		REQUIRE(result1 == std::vector<int>{1, 2});
	}

	SECTION("Graph with High Precision Floating Point Nodes") {
		auto g_float = gdwg::graph<double, double>{};
		g_float.insert_node(1.0000000001);
		g_float.insert_node(1.0000000002);
		g_float.insert_node(1.0000000003);
		auto const& result = g_float.nodes();
		REQUIRE(result.size() == 3);
		// Ensure precise floats are sorted correctly
		REQUIRE(result[0] < result[1]);
		REQUIRE(result[1] < result[2]);
	}

	SECTION("Check the return is copy from graph") {
		g.insert_node(3);
		g.insert_node(2);
		g.insert_node(1);
		auto const& result = g.nodes();
		g.clear();
		auto const& af_clear = g.nodes();
		REQUIRE(result != af_clear);
		REQUIRE(af_clear.empty());
		REQUIRE(result[0] == 1);
		REQUIRE(result.size() == 3);
		auto const& expect_result = std::vector<int>{1, 2, 3};
		REQUIRE(result == expect_result);
	}
}

TEST_CASE("Graph replace_node operation", "[replace_node]") {
	auto g = gdwg::graph<int, double>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);
	g.insert_node(4);

	SECTION("Successful Node Replacement") {
		auto const replaced = g.replace_node(2, 5);
		REQUIRE(replaced);
		REQUIRE(g.is_node(5));
		// 2 should no longer exist
		REQUIRE_FALSE(g.is_node(2));
		auto const& nodes = g.nodes();
		// Ensure 2 is not in nodes list
		REQUIRE(std::find(nodes.begin(), nodes.end(), 2) == nodes.end());
		// Ensure 4 is in nodes list
		REQUIRE(std::find(nodes.begin(), nodes.end(), 5) != nodes.end());
	}

	SECTION("Replace With Existing Node") {
		auto const replaced = g.replace_node(2, 1);
		REQUIRE_FALSE(replaced);
		REQUIRE(g.is_node(2));
		REQUIRE(g.is_node(1));
	}

	SECTION("Replace Non-Existent Node") {
		REQUIRE_THROWS_MATCHES(g.replace_node(5, 6),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node on a node that "
		                                                "doesn't exist"));
	}

	SECTION("Replace Node with Same Value") {
		auto const replaced = g.replace_node(3, 3);
		REQUIRE_FALSE(replaced);
		REQUIRE(g.is_node(3));
	}

	SECTION("Node Replacement Effects on Edges") {
		g.insert_edge(1, 2);
		g.insert_edge(2, 3);
		auto const replaced = g.replace_node(2, 7);
		REQUIRE(replaced);
		REQUIRE_FALSE(g.is_node(2));
		REQUIRE(g.is_node(7));
		REQUIRE(g.is_connected(1, 7));
		REQUIRE(g.is_connected(7, 3));
	}

	SECTION("Replace after move") {
		g.insert_edge(1, 2);
		g.insert_edge(2, 3);
		auto const ng = g;
		auto const replaced = g.replace_node(2, 7);
		REQUIRE(replaced);
		REQUIRE_FALSE(g.is_node(2));
		REQUIRE(ng != g);
		auto gos = std::ostringstream{};
		gos << g;
		auto ngos = std::ostringstream{};
		ngos << ng;
		REQUIRE(gos.str() != ngos.str());
		auto constexpr expected_ngos = std::string_view(R"(
1 (
  1 -> 2 | U
)
2 (
  2 -> 3 | U
)
3 (
)
4 (
)
)");
		REQUIRE(ngos.str() == expected_ngos);
	}
}

TEST_CASE("Graph merge_replace_node operation", "[merge_replace_node]") {
	auto g = gdwg::graph<std::string, double>{};
	g.insert_node("A");
	g.insert_node("B");
	g.insert_node("C");
	g.insert_node("D");

	g.insert_edge("A", "B", 1.0);
	g.insert_edge("A", "C", 2.0);
	g.insert_edge("A", "D", 3.0);
	g.insert_edge("B", "B", 1.0);

	SECTION("Normal Merge Replace Node") {
		g.merge_replace_node("A", "B");
		// Node A (1) should be replaced by Node B (2)
		REQUIRE(g.is_node("B"));
		REQUIRE_FALSE(g.is_node("A"));

		REQUIRE(g.is_connected("B", "B"));
		REQUIRE(g.is_connected("B", "C"));
		REQUIRE(g.is_connected("B", "D"));
		auto const& nodes = g.nodes();
		// Node list should now contain B, C, D only
		REQUIRE(nodes.size() == 3);
		// A should not exist
		REQUIRE(std::find(nodes.begin(), nodes.end(), "A") == nodes.end());
		// B should exist
		REQUIRE(std::find(nodes.begin(), nodes.end(), "B") != nodes.end());
	}

	SECTION("Merge Replace Node With Existing Duplicate Edges") {
		// Add duplicate edge for when A is replaced by B
		g.insert_edge("B", "C", 2.0);
		g.merge_replace_node("A", "B");
		REQUIRE(g.is_node("B"));
		// Node A (1) should be replaced
		REQUIRE_FALSE(g.is_node("A"));
		REQUIRE(g.is_connected("B", "B"));
		REQUIRE(g.is_connected("B", "C"));
		REQUIRE(g.is_connected("B", "D"));
		// Should only have one edge B->C with weight 2.0, duplicates removed
		REQUIRE(g.edges("B", "C").size() == 1);
	}

	SECTION("Invalid Merge Replace Due to Non-Existent Source Node") {
		REQUIRE_THROWS_MATCHES(g.merge_replace_node("E", "B"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist "
		                                                "in the graph"));
	}

	SECTION("Invalid Merge Replace Due to Non-Existent Target Node") {
		REQUIRE_THROWS_MATCHES(g.merge_replace_node("A", "E"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist "
		                                                "in the graph"));
	}

	SECTION("Merge Replace Node to Itself") {
		// Replacing node with itself
		g.merge_replace_node("A", "A");
		REQUIRE(g.is_node("A"));
		REQUIRE(g.is_connected("A", "B"));
		REQUIRE(g.is_connected("A", "C"));
		REQUIRE(g.is_connected("A", "D"));
		// No duplicates should exist
		REQUIRE(g.edges("A", "B").size() == 1);
	}
}

TEST_CASE("Graph erase_node operation", "[erase_node]") {
	auto g = gdwg::graph<char, double>{};
	g.insert_node('A');
	g.insert_node('B');
	g.insert_node('C');
	g.insert_node('D');

	g.insert_edge('A', 'B', 1.0);
	g.insert_edge('B', 'C', 2.0);
	g.insert_edge('C', 'A', 3.0);
	g.insert_edge('A', 'A', 4.0);

	SECTION("Erase Existing Node with Edges") {
		auto const erased = g.erase_node('A');
		REQUIRE(erased);
		REQUIRE_FALSE(g.is_node('A'));
		REQUIRE_THROWS_MATCHES(g.is_connected('A', 'B'),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.is_connected('C', 'A'),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
		// Self-loop should also be removed
		REQUIRE_THROWS_MATCHES(g.is_connected('A', 'A'),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
	}

	SECTION("Erase Nonexistent Node") {
		auto const erased = g.erase_node('E');
		REQUIRE_FALSE(erased);
	}

	SECTION("Erase Node After Merge Replace Operation") {
		g.merge_replace_node('A', 'B');
		auto const erased = g.erase_node('A');
		REQUIRE_FALSE(erased);
		REQUIRE(g.is_node('B'));
		REQUIRE(g.is_connected('B', 'B'));
		REQUIRE(g.is_connected('B', 'C'));
	}

	SECTION("Erase Node After Replace Node Operation") {
		g.replace_node('A', 'E');
		auto const erased = g.erase_node('A');
		REQUIRE_FALSE(erased);
		REQUIRE(g.is_node('E'));
		REQUIRE(g.is_connected('E', 'B'));
		REQUIRE(g.is_connected('C', 'E'));
	}

	SECTION("Erase Node and Check Graph Integrity") {
		g.erase_node('C');
		REQUIRE_FALSE(g.is_node('C'));
		auto const& nodes = g.nodes();
		// Only Nodes 1, 2, 4 should remain
		REQUIRE(nodes.size() == 3);
		// Node 3 should not be in the list
		REQUIRE(std::find(nodes.begin(), nodes.end(), 'C') == nodes.end());
	}
}

TEST_CASE("Graph erase_edge operation", "[erase_edge]") {
	auto g = gdwg::graph<double, double>{};
	g.insert_node(1); // Edge A
	g.insert_node(2); // Edge B
	g.insert_node(3); // Edge C
	g.insert_edge(1, 2, 1.0); // Edge A->B with weight
	g.insert_edge(1, 3); // Unweighted edge A->C

	SECTION("Erase Weighted Edge") {
		// Ensure edge exists before removal
		REQUIRE(g.is_connected(1, 2));
		auto const erased = g.erase_edge(1, 2, 1.0);
		REQUIRE(erased);
		// Edge should no longer exist
		REQUIRE_FALSE(g.is_connected(1, 2));
	}

	SECTION("Erase Unweighted Edge") {
		REQUIRE(g.is_connected(1, 3));
		auto const erased = g.erase_edge(1, 3);
		REQUIRE(erased);
		REQUIRE_FALSE(g.is_connected(1, 3));
	}

	SECTION("Attempt to Erase Non-Existent Edge") {
		auto const erased = g.erase_edge(1, 2, 2.0);
		REQUIRE_FALSE(erased);
	}

	SECTION("Attempt to Erase Edge with Non-Existent Nodes") {
		REQUIRE_THROWS_MATCHES(g.erase_edge(4, 5),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
		                                                "they don't exist in the graph"));
	}

	SECTION("Check not erase all edges between two nodes") {
		REQUIRE(g.insert_edge(1, 2, 2.0));
		REQUIRE(g.erase_edge(1, 2, 1.0));
		REQUIRE(g.is_connected(1, 2));
		REQUIRE(g.insert_edge(1, 2));
		REQUIRE(g.erase_edge(1, 2, 2.0));
		REQUIRE(g.is_connected(1, 2));
		REQUIRE(g.erase_edge(1, 2));
		REQUIRE_FALSE(g.is_connected(1, 2));
		REQUIRE_FALSE(g.erase_edge(1, 2));
	}
}

TEST_CASE("Graph is_connected operation", "[is_connected]") {
	auto g = gdwg::graph<int, double>{};
	g.insert_node(1); // Edge A
	g.insert_node(2); // Edge B
	g.insert_node(3); // Edge C
	g.insert_edge(1, 2, 1.0);

	SECTION("Check Connected Nodes") {
		// Weighted connection
		REQUIRE(g.is_connected(1, 2));
	}

	SECTION("Check Unconnected Nodes") {
		REQUIRE_FALSE(g.is_connected(2, 3));
	}

	SECTION("Check Connection After Edge Removal") {
		g.erase_edge(1, 2, 1.0);
		REQUIRE_FALSE(g.is_connected(1, 2));
	}

	SECTION("Check Connection with Non-Existent Nodes") {
		REQUIRE_THROWS_MATCHES(g.is_connected(4, 5),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
	}
}

TEST_CASE("Graph Iterator Functionality", "[iterator]") {
	SECTION("Type traits") {
		using iter_class = gdwg::graph<std::string, int>::iterator;
		STATIC_REQUIRE(std::is_same_v<iter_class::pointer, void>);
		STATIC_REQUIRE(
		    std::is_same_v<std::iterator_traits<iter_class>::iterator_category, std::bidirectional_iterator_tag>);
		STATIC_REQUIRE(std::is_same_v<iter_class::difference_type, std::ptrdiff_t>);
	}

	SECTION("Check default constructor") {
		REQUIRE_NOTHROW(gdwg::graph<std::string, double>::iterator{});
		auto const& it1 = gdwg::graph<std::string, double>::iterator{};
		auto const& it2 = gdwg::graph<std::string, double>::iterator{};
		REQUIRE(it1 == it2);
	}

	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(7);
	g.insert_node(12);
	g.insert_node(19);
	g.insert_node(21);
	g.insert_node(14);
	g.insert_node(31);

	g.insert_edge(1, 7, 4);
	g.insert_edge(19, 1, 3);
	g.insert_edge(1, 12, 3);
	g.insert_edge(7, 21, 13);
	g.insert_edge(1, 12);
	g.insert_edge(12, 19, 16);
	g.insert_edge(21, 14, 23);
	g.insert_edge(14, 14, 0);
	g.insert_edge(19, 21, 2);
	g.insert_edge(21, 31, 14);

	SECTION("Iterate through all edges") {
		auto it = g.begin();
		REQUIRE((*it).from == 1);
		REQUIRE((*it).to == 7);
		REQUIRE((*it).weight == 4);
		++it;
		REQUIRE((*it).from == 1);
		REQUIRE((*it).to == 12);
		REQUIRE((*it).weight == std::nullopt);
		++it;
		REQUIRE((*it).from == 1);
		REQUIRE((*it).to == 12);
		REQUIRE((*it).weight == 3);
	}

	SECTION("End iterator") {
		auto g2 = gdwg::graph<int, int>{};
		g2.insert_node(1);
		g2.insert_node(7);
		g2.insert_node(12);

		g2.insert_edge(7, 12);
		auto it = g2.begin();
		REQUIRE_FALSE(it == g2.end());
		REQUIRE((*it).from == 7);
		REQUIRE((*it).to == 12);
		REQUIRE((*it).weight == std::nullopt);
		++it;
		REQUIRE(it == g2.end());
	}

	SECTION("Empty graph") {
		auto const& empty_graph = gdwg::graph<int, int>{};
		auto const& it = empty_graph.begin();
		REQUIRE(it == empty_graph.end());
	}

	SECTION("Reverse iteration using decrement") {
		auto it = g.end();
		--it;
		REQUIRE((*it).from == 21);
		REQUIRE((*it).to == 31);
		REQUIRE((*it).weight == 14);
		--it;
		REQUIRE((*it).from == 21);
		REQUIRE((*it).to == 14);
		REQUIRE((*it).weight == 23);
	}

	SECTION("Iterator comparison") {
		auto start = g.begin();
		auto const& finish = g.end();
		REQUIRE(start != finish);
		++start;
		REQUIRE(start != g.begin());
	}

	SECTION("Post-increment and post-decrement operators") {
		auto it = g.begin();
		auto const old_it = it++;
		REQUIRE(old_it == g.begin());
		auto const last_it = --it;
		REQUIRE(last_it == g.begin());
	}
}

TEST_CASE("Graph erase_edge(iterator) operation", "[erase_edge]") {
	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2, 5);
	g.insert_edge(1, 3, 10);
	g.insert_edge(2, 3, 15);

	SECTION("Normal Operation - Erase Edge") {
		// Points to edge (1, 2, 5)
		auto const& it = g.begin();
		// Should point to edge (1, 3, 10)
		auto const& next_it = std::next(it);
		auto const& returned_it = g.erase_edge(it);
		REQUIRE(returned_it == next_it);
		REQUIRE_FALSE(g.is_connected(1, 2));
	}

	SECTION("Edge Case - Erase Last Edge") {
		// Assuming this is the iterator pointing to the last edge (2, 3, 15)
		auto const& it = std::next(g.begin(), 2);
		auto const& returned_it = g.erase_edge(it);
		REQUIRE(returned_it == g.end());
		REQUIRE_FALSE(g.is_connected(2, 3));
	}
}

TEST_CASE("Graph erase_edge(iterator i, iterator s) operation", "[erase_edge_range]") {
	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);
	g.insert_node(4);

	g.insert_edge(1, 2, 5);
	g.insert_edge(1, 3, 10);
	g.insert_edge(1, 4, 15);
	g.insert_edge(2, 3, 20);
	g.insert_edge(3, 4, 25);

	SECTION("Erase multiple edges in a range") {
		auto const& start = g.begin();
		auto const& end = std::next(start, 3);

		auto const& result = g.erase_edge(start, end);
		REQUIRE(result == end);
		REQUIRE_FALSE(g.is_connected(1, 2));
		REQUIRE_FALSE(g.is_connected(1, 3));
		REQUIRE_FALSE(g.is_connected(1, 4));
		REQUIRE(g.is_connected(2, 3));
	}

	SECTION("Erase range up to the end") {
		auto const& start = g.begin();
		auto const& end = g.end();
		auto const& result = g.erase_edge(start, end);
		REQUIRE(result == g.end());
		REQUIRE(g.begin() == g.end());
	}

	SECTION("Empty range does nothing") {
		auto const& start = g.begin();
		auto const& result = g.erase_edge(start, start);
		REQUIRE(result == start);
		REQUIRE(g.begin() != g.end());
	}

	SECTION("Erase entire graph") {
		auto const& start = g.begin();
		auto const& end = g.end();
		g.erase_edge(start, end);
		REQUIRE(g.begin() == g.end());
	}
}

TEST_CASE("Graph clear operation", "[clear]") {
	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2, 100);
	g.insert_edge(2, 3, 200);
	g.insert_edge(3, 1, 300);

	SECTION("Clearing non-empty graph") {
		REQUIRE_FALSE(g.empty());
		g.clear();
		REQUIRE(g.empty());
		REQUIRE(g.begin() == g.end());
	}

	SECTION("Clearing already empty graph") {
		g.clear();
		REQUIRE(g.empty());
		g.clear();
		REQUIRE(g.empty());
	}

	SECTION("Clear and add nodes again") {
		g.clear();
		REQUIRE(g.empty());
		g.insert_node(4);
		REQUIRE_FALSE(g.empty());
		REQUIRE(g.is_node(4));
	}
}

TEST_CASE("Graph edges operation", "[edges]") {
	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2);
	g.insert_edge(1, 2, 5);
	g.insert_edge(1, 2, 3);

	SECTION("Retrieve edges in specified order") {
		auto const& es = g.edges(1, 2);
		REQUIRE(es.size() == 3);
		REQUIRE(es[0]->print_edge() == "1 -> 2 | U");
		REQUIRE(es[1]->print_edge() == "1 -> 2 | W | 3");
		REQUIRE(es[2]->print_edge() == "1 -> 2 | W | 5");
	}

	SECTION("Invalid cases - Nonexistent source or destination") {
		REQUIRE_THROWS_MATCHES(g.edges(1, 4),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
		REQUIRE_THROWS_MATCHES(g.edges(4, 2),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
	}

	SECTION("Replace node and check edges") {
		REQUIRE(g.replace_node(2, 4));
		REQUIRE_THROWS_MATCHES(g.edges(1, 2),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
		auto const& es = g.edges(1, 4);
		// All edges previously going to '2' should now go to '4'
		REQUIRE(es.size() == 3);
	}

	SECTION("Merge replace node and check edges") {
		REQUIRE(g.insert_edge(1, 3));
		REQUIRE(g.insert_edge(1, 3, 3));
		REQUIRE(g.insert_edge(1, 3, 7));
		// Merging '2' into '3', expecting combined edges without duplicates
		g.merge_replace_node(2, 3);
		auto const& es = g.edges(1, 3);
		// Should see three weighted (3, 5, 7) and one unweighted edge
		// REQUIRE(es.size() == 4);
		REQUIRE(es[0]->print_edge() == "1 -> 3 | U");
		REQUIRE(es[1]->print_edge() == "1 -> 3 | W | 3");
		REQUIRE(es[2]->print_edge() == "1 -> 3 | W | 5");
		REQUIRE(es[3]->print_edge() == "1 -> 3 | W | 7");
	}

	SECTION("Clear graph and check edge persistence") {
		// Get edges before clear
		auto const& es = g.edges(1, 2);
		g.clear();
		REQUIRE(g.empty());
		// The vector of edges should still persist its data
		REQUIRE(es.size() == 3);
		REQUIRE(es[0]->print_edge() == "1 -> 2 | U");
		REQUIRE(es[1]->print_edge() == "1 -> 2 | W | 3");
		REQUIRE(es[2]->print_edge() == "1 -> 2 | W | 5");
	}
}

TEST_CASE("Graph find operation", "[find]") {
	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2, 100);
	g.insert_edge(1, 3, 200);
	g.insert_edge(2, 3, 300);

	SECTION("Find existing weighted edge") {
		auto const& it = g.find(1, 2, 100);
		REQUIRE(it != g.end());
		REQUIRE((*it).from == 1);
		REQUIRE((*it).to == 2);
		REQUIRE((*it).weight == 100);
		REQUIRE(it == g.begin());
	}

	SECTION("Find with no matching weight") {
		auto const& it = g.find(1, 2, 101);
		REQUIRE(it == g.end());
	}

	SECTION("Find unweighted edge which doesn't exist") {
		auto const& it = g.find(1, 2, std::nullopt);
		REQUIRE(it == g.end());
	}

	SECTION("Find non-existent edge between existing nodes") {
		auto const& it = g.find(1, 3, 300);
		REQUIRE(it == g.end());
	}

	SECTION("Find edge between non-existent nodes") {
		g.insert_node(4);
		auto const& it = g.find(1, 4, std::nullopt);
		REQUIRE(it == g.end());
	}

	SECTION("Find in empty graph") {
		auto empty_graph = gdwg::graph<int, int>{};
		auto const& it = empty_graph.find(1, 2, 100);
		REQUIRE(it == empty_graph.end());
	}
}

TEST_CASE("Graph connections operation", "[connections]") {
	auto g = gdwg::graph<int, int>{};
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);
	g.insert_node(4);

	g.insert_edge(1, 2, 100);
	g.insert_edge(1, 3, 200);
	g.insert_edge(1, 4, 300);
	g.insert_edge(2, 3, 400);

	SECTION("Valid connections retrieval") {
		auto const& result = g.connections(1);
		auto const& expected = std::vector<int>{2, 3, 4};
		REQUIRE(result == expected);
	}

	SECTION("No outgoing edges") {
		auto const& result = g.connections(4);
		REQUIRE(result.empty());
	}

	SECTION("Multiple edges to the same node") {
		g.insert_edge(1, 2, 500);
		auto const& result = g.connections(1);
		// Node 2 should still appear only once
		auto const& expected = std::vector<int>{2, 3, 4};
		REQUIRE(result == expected);
	}

	SECTION("Node does not exist") {
		REQUIRE_THROWS_MATCHES(g.connections(5),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if src doesn't "
		                                                "exist in the graph"));
	}

	SECTION("Check sorted order of connections") {
		g.insert_edge(1, 3, 600);
		auto const& result = g.connections(1);
		// Should still maintain sorted order
		auto const& expected = std::vector<int>{2, 3, 4};
		REQUIRE(result == expected);
	}

	SECTION("Empty graph") {
		auto empty_graph = gdwg::graph<int, int>{};
		REQUIRE_THROWS_MATCHES(empty_graph.connections(1),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if src doesn't "
		                                                "exist in the graph"));
	}

	SECTION("No duplicate node in vector") {
		g.insert_edge(1, 4);
		g.insert_edge(1, 3);
		g.insert_edge(1, 2);
		g.insert_edge(1, 2, 10);
		g.insert_edge(1, 3, 20);
		g.insert_edge(1, 4, 30);
		auto const& result = g.connections(1);
		auto const& expected = std::vector<int>{2, 3, 4};
		REQUIRE(result == expected);
	}
}

TEST_CASE("Graph equality operation", "[operator==]") {
	auto g1 = gdwg::graph<int, int>{};
	auto g2 = gdwg::graph<int, int>{};

	g1.insert_node(1);
	g1.insert_node(2);
	g1.insert_edge(1, 2, 100);

	g2.insert_node(1);
	g2.insert_node(2);
	g2.insert_edge(1, 2, 100);

	SECTION("Identical graphs") {
		REQUIRE(g1 == g2);
	}

	SECTION("Graphs with same nodes but different edges") {
		g2.insert_edge(1, 2, 200);
		REQUIRE_FALSE(g1 == g2);
		REQUIRE(g1 != g2);
	}

	SECTION("Graphs with different nodes") {
		g2.insert_node(3);
		REQUIRE_FALSE(g1 == g2);
		REQUIRE(g1 != g2);
	}

	SECTION("Graphs with same nodes but one has extra edge") {
		g2.insert_edge(2, 1, 300);
		REQUIRE_FALSE(g1 == g2);
		REQUIRE(g1 != g2);
	}

	SECTION("Comparing graph to itself") {
		REQUIRE(g1 == g1);
	}

	SECTION("Empty graphs") {
		auto const& empty1 = gdwg::graph<int, int>{};
		auto const& empty2 = gdwg::graph<int, int>{};
		REQUIRE(empty1 == empty2);
	}

	SECTION("Empty graph and non-empty graph") {
		auto const& empty = gdwg::graph<int, int>{};
		REQUIRE_FALSE(g1 == empty);
		REQUIRE(g1 != empty);
	}

	SECTION("Equal for copy graph") {
		auto const g3 = gdwg::graph<int, int>{g1};
		REQUIRE(g3 == g1);
		REQUIRE(g3 == g2);
		auto g4 = gdwg::graph<int, int>{};
		g4 = g1;
		REQUIRE(g4 == g1);
		REQUIRE(g4 == g2);
	}
}

TEST_CASE("Spec test for operator<<", "[operator<<]") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 1, 1},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};

	auto g = graph{};
	for (auto const& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}
	g.insert_node(64);

	auto out = std::ostringstream{};
	out << g;
	auto constexpr expected_output = std::string_view(R"(
1 (
  1 -> 5 | W | -1
)
2 (
  2 -> 1 | W | 1
  2 -> 4 | U
)
3 (
  3 -> 2 | W | 2
  3 -> 6 | W | -8
)
4 (
  4 -> 1 | W | -4
  4 -> 5 | W | 3
)
5 (
  5 -> 2 | U
)
6 (
  6 -> 2 | W | 5
  6 -> 3 | W | 10
)
64 (
)
)");
	REQUIRE(out.str() == expected_output);
}

TEST_CASE("Internal representation") {
	auto g = gdwg::graph<std::string, int>{};
	{
		auto const& s1 = std::string{"Hello"};
		g.insert_node(s1);
	}
	REQUIRE(g.is_node("Hello"));
}

TEST_CASE("Graph output operator", "[operator<<]") {
	auto g = gdwg::graph<int, int>{};
	auto out = std::ostringstream{};
	SECTION("Empty graph") {
		out << g;
		REQUIRE(out.str() == "\n");
	}

	SECTION("Graph with only nodes") {
		g.insert_node(10);
		g.insert_node(20);

		out << g;
		auto constexpr expected_output = std::string_view("\n"
		                                                  "10 (\n)\n"
		                                                  "20 (\n)\n");
		REQUIRE(out.str() == expected_output);
	}

	SECTION("Graph with unweighted and weighted edges") {
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);
		g.insert_edge(1, 2);
		g.insert_edge(2, 3, 5);

		out << g;
		auto constexpr expected_output = std::string_view("\n"
		                                                  "1 (\n"
		                                                  "  1 -> 2 | U\n"
		                                                  ")\n"
		                                                  "2 (\n"
		                                                  "  2 -> 3 | W | 5\n"
		                                                  ")\n"
		                                                  "3 (\n)\n");
		REQUIRE(out.str() == expected_output);
	}

	SECTION("Graph with mixed connections") {
		g.insert_node(1);
		g.insert_node(2);
		g.insert_edge(1, 1);
		g.insert_edge(1, 2, 3);

		out << g;
		auto constexpr expected_output = std::string_view("\n"
		                                                  "1 (\n"
		                                                  "  1 -> 1 | U\n"
		                                                  "  1 -> 2 | W | 3\n"
		                                                  ")\n"
		                                                  "2 (\n)\n");
		REQUIRE(out.str() == expected_output);
	}

	SECTION("Graph with nodes but no edges") {
		g.insert_node(1);
		g.insert_node(2);

		out << g;
		auto constexpr expected_output = std::string_view("\n"
		                                                  "1 (\n)\n"
		                                                  "2 (\n)\n");
		REQUIRE(out.str() == expected_output);
	}
}

TEST_CASE("New operator<< test") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 4, 2},
	    {2, 1, 1},
	    {4, 1, std::nullopt},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}
	g.insert_node(64);

	auto out = std::ostringstream{};
	out << g;
	auto constexpr expected_output = std::string_view(R"(
1 (
  1 -> 5 | W | -1
)
2 (
  2 -> 1 | W | 1
  2 -> 4 | U
  2 -> 4 | W | 2
)
3 (
  3 -> 2 | W | 2
  3 -> 6 | W | -8
)
4 (
  4 -> 1 | U
  4 -> 1 | W | -4
  4 -> 5 | W | 3
)
5 (
  5 -> 2 | U
)
6 (
  6 -> 2 | W | 5
  6 -> 3 | W | 10
)
64 (
)
)");
	REQUIRE(out.str() == expected_output);
}

TEST_CASE("Comprehensive Test of Graph<char, int>", "[graph]") {
	auto constexpr node_a = 'a';
	auto constexpr node_b = 'b';
	auto constexpr node_c = 'c';
	auto constexpr node_d = 'd';
	auto constexpr new_node = 'n';
	auto graph = gdwg::graph<char, int>{};

	// Node insertion
	REQUIRE(graph.insert_node(node_a));
	REQUIRE(graph.insert_node(node_b));
	REQUIRE(graph.insert_node(node_c));
	REQUIRE(graph.insert_node(node_d));

	// Edge insertion
	// Weighted
	REQUIRE(graph.insert_edge(node_a, node_b, 5));
	// Unweighted
	REQUIRE(graph.insert_edge(node_a, node_c));
	// Weighted
	REQUIRE(graph.insert_edge(node_b, node_d, 10));

	SECTION("Insert False") {
		REQUIRE_FALSE(graph.insert_node(node_a));
		REQUIRE_FALSE(graph.insert_node(node_b));
		REQUIRE_FALSE(graph.insert_node(node_c));
		REQUIRE_FALSE(graph.insert_node(node_d));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_b, 5));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_c));
		REQUIRE_FALSE(graph.insert_edge(node_b, node_d, 10));
	}

	SECTION("Verify Node and Edge Insertions") {
		REQUIRE(graph.is_node(node_a));
		REQUIRE(graph.is_node(node_d));
		REQUIRE(graph.is_connected(node_a, node_b));
		REQUIRE(graph.is_connected(node_a, node_c));
		REQUIRE_FALSE(graph.is_connected(node_a, node_d));
	}

	SECTION("Test Edge Erasure and Node Erasure") {
		REQUIRE(graph.erase_edge('a', 'b', 5));
		REQUIRE_FALSE(graph.is_connected('a', 'b'));
		REQUIRE(graph.erase_node('d'));
		REQUIRE_FALSE(graph.is_node('d'));
	}

	SECTION("Test Output of Graph") {
		auto oss = std::ostringstream{};
		oss << graph;
		auto constexpr expected_output = std::string_view("\na (\n"
		                                                  "  a -> b | W | 5\n"
		                                                  "  a -> c | U\n"
		                                                  ")\n"
		                                                  "b (\n"
		                                                  "  b -> d | W | 10\n"
		                                                  ")\n"
		                                                  "c (\n)\n"
		                                                  "d (\n)\n");
		REQUIRE(oss.str() == expected_output);
	}

	SECTION("Test Graph Equality") {
		auto graph_copy = graph;
		REQUIRE(graph == graph_copy);
		graph_copy.insert_edge(node_a, node_d, 3);
		REQUIRE(graph != graph_copy);
	}

	SECTION("Replace Node and Check Edges") {
		auto const it = graph.find(node_a, node_b, 5);
		REQUIRE(it != graph.end());
		REQUIRE(it == graph.begin());
		auto const& [from, to, weight] = *it;
		REQUIRE(from == node_a);
		REQUIRE(to == node_b);
		REQUIRE(weight == 5);
		REQUIRE(graph.connections(node_a) == std::vector<char>{node_b, node_c});
		REQUIRE(std::next(it, 3) == graph.end());

		REQUIRE_THROWS_MATCHES(graph.replace_node(new_node, new_node),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node on a node that "
		                                                "doesn't exist"));
		REQUIRE_FALSE(graph.replace_node(node_b, node_a));

		REQUIRE(graph.replace_node(node_b, new_node));
		REQUIRE(graph.connections(node_a) == std::vector<char>{node_c, new_node});
		REQUIRE(graph.find(node_a, node_b, 5) == graph.end());

		REQUIRE_THROWS_MATCHES(graph.is_connected(node_a, node_b),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
		REQUIRE(graph.is_connected(node_a, new_node));
		REQUIRE_THROWS_MATCHES(graph.connections(node_b),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if src doesn't "
		                                                "exist in the graph"));
	}

	SECTION("Merge Replace Node and Check Edges") {
		auto const& edges_bf = graph.edges(node_a, node_c);
		REQUIRE(edges_bf.size() == 1);
		REQUIRE(edges_bf[0]->get_nodes() == std::make_pair(node_a, node_c));
		REQUIRE(edges_bf[0]->get_weight() == std::nullopt);
		REQUIRE_FALSE(edges_bf[0]->is_weighted());
		REQUIRE(edges_bf[0]->print_edge() == std::string{"a -> c | U"});

		REQUIRE(graph.insert_edge(node_c, node_d, 20));
		REQUIRE(graph.insert_edge(node_b, node_c, 1));
		graph.merge_replace_node(node_b, node_c);
		REQUIRE(graph.is_connected(node_a, node_c));

		auto edges = graph.edges(node_a, node_c);
		REQUIRE(edges.size() == 2);
		REQUIRE(*edges[0] == *edges_bf[0]);
		REQUIRE(edges[1]->get_nodes() == std::make_pair(node_a, node_c));
		REQUIRE(edges[1]->get_weight() == 5);
		REQUIRE(edges[1]->is_weighted());
		REQUIRE(edges[1]->print_edge() == std::string{"a -> c | W | 5"});

		auto oss = std::ostringstream{};
		oss << graph;
		auto constexpr expected_output = std::string_view("\na (\n"
		                                                  "  a -> c | U\n"
		                                                  "  a -> c | W | 5\n"
		                                                  ")\n"
		                                                  "c (\n"
		                                                  "  c -> c | W | 1\n"
		                                                  "  c -> d | W | 10\n"
		                                                  "  c -> d | W | 20\n"
		                                                  ")\n"
		                                                  "d (\n)\n");
		REQUIRE(oss.str() == expected_output);

		REQUIRE_THROWS_MATCHES(graph.merge_replace_node(node_b, node_c),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(graph.merge_replace_node(node_c, node_b),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
	}

	SECTION("Clear graph and Check Edge and Nodes Persistence") {
		auto const& edges = graph.edges(node_a, node_b);
		auto const& nodes = graph.nodes();
		graph.clear();
		REQUIRE(graph.empty());
		// Ensure edges vector still holds its data
		REQUIRE(edges.size() == 1);
		REQUIRE(nodes.size() == 4);
		REQUIRE(nodes == std::vector<char>{node_a, node_b, node_c, node_d});
	}

	SECTION("Erase Node") {
		REQUIRE(graph.erase_node(node_a));
		REQUIRE_FALSE(graph.erase_node(node_a));
		REQUIRE(graph.erase_node(node_b));
		REQUIRE(graph.erase_node(node_c));
		REQUIRE(graph.erase_node(node_d));
		REQUIRE(graph.empty());
	}

	SECTION("Erase Edge") {
		REQUIRE(graph.erase_edge(node_a, node_c));
		REQUIRE_FALSE(graph.erase_edge(node_a, node_c));
		REQUIRE_THROWS_MATCHES(graph.erase_edge(new_node, node_c),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
		                                                "they don't exist in the graph"));
		REQUIRE_FALSE(graph.erase_edge(node_a, node_b));
		REQUIRE(graph.erase_edge(node_a, node_b, 5));
	}

	SECTION("Erase Edge Iterator") {
		auto distance_bf = std::distance(graph.begin(), graph.end());
		auto it = graph.erase_edge(graph.begin());
		REQUIRE(it == graph.begin());
		REQUIRE_FALSE(std::distance(graph.begin(), graph.end()) == distance_bf);
		it = graph.erase_edge(std::next(it), std::prev(graph.end()));
		REQUIRE(it == std::prev(graph.end()));
		REQUIRE((it == std::next(graph.begin())));
		graph.erase_edge(graph.begin(), graph.end());
		REQUIRE(graph.begin() == graph.end());
	}

	SECTION("Test const method of copy constructor") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		// Should don't have a same iterator, but same value.
		REQUIRE(ng.begin() != graph.begin());
		auto const [ng_from, ng_to, ng_weight] = *ng.begin();
		auto const [from, to, weight] = *graph.begin();
		REQUIRE(ng_from == from);
		REQUIRE(ng_to == to);
		REQUIRE(ng_weight == weight);
		REQUIRE(std::distance(ng.begin(), ng.end()) == std::distance(graph.begin(), graph.end()));

		// Accessor should can running by const
		REQUIRE(ng.is_node(node_a) == graph.is_node(node_a));
		REQUIRE(ng.is_node(new_node) == graph.is_node(new_node));
		REQUIRE(ng.empty() == graph.empty());
		REQUIRE(ng.nodes() == graph.nodes());
		REQUIRE(ng.is_connected(node_a, node_b) == graph.is_connected(node_a, node_b));
		REQUIRE(ng.find(node_c, node_d) == ng.end());
		REQUIRE(ng.connections(node_a) == graph.connections(node_a));

		// But not edge because it is a vector of pointer.
		REQUIRE(ng.edges(node_a, node_b) != graph.edges(node_a, node_b));
	}

	SECTION("Const copy constructor and move constructor") {
		auto const copy = graph;
		auto const move = std::move(graph);
		REQUIRE(copy == move);
		auto out_copy = std::ostringstream{};
		out_copy << copy;
		auto out_move = std::ostringstream{};
		out_move << move;
		REQUIRE(out_copy.str() == out_move.str());
	}

	SECTION("Invalid Operations") {
		// Both nodes don't exist
		REQUIRE_THROWS_MATCHES(graph.edges('e', 'f'),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
		// 'e' does not exist
		REQUIRE_THROWS_MATCHES(graph.insert_edge('a', 'e', 20),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                                                "dst node does not exist"));
		// Node 'e' does not exist
		REQUIRE_FALSE(graph.erase_node('e'));
	}
}

TEST_CASE("Limited Test of Graph<string, double>", "[graph]") {
	auto constexpr node_a = "a";
	auto constexpr node_b = "b";
	auto constexpr node_c = "c";
	auto constexpr node_d = "d";
	auto constexpr new_node = "n";
	auto graph = gdwg::graph<std::string, double>{};

	// Node insertion
	REQUIRE(graph.insert_node(node_a));
	REQUIRE(graph.insert_node(node_b));
	REQUIRE(graph.insert_node(node_c));
	REQUIRE(graph.insert_node(node_d));

	// Edge insertion with various double edge cases
	REQUIRE(graph.insert_edge(node_a, node_b, 5.0));
	REQUIRE(graph.insert_edge(node_a, node_c, std::numeric_limits<double>::infinity()));
	REQUIRE(graph.insert_edge(node_a, node_c, -std::numeric_limits<double>::infinity()));
	REQUIRE(graph.insert_edge(node_a, node_c, 3.000001));
	REQUIRE(graph.insert_edge(node_a, node_c, 3.0000001));
	REQUIRE(graph.insert_edge(node_b, node_d, -std::numeric_limits<double>::infinity()));
	REQUIRE(graph.insert_edge(node_c, node_d, 3.000001));
	REQUIRE(graph.insert_edge(node_d, node_a, 3.0000001));

	SECTION("Insert False") {
		REQUIRE_FALSE(graph.insert_node(node_a));
		REQUIRE_FALSE(graph.insert_node(node_b));
		REQUIRE_FALSE(graph.insert_node(node_c));
		REQUIRE_FALSE(graph.insert_node(node_d));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_b, 5.0));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_c, std::numeric_limits<double>::infinity()));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_c, -std::numeric_limits<double>::infinity()));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_c, 3.000001));
		REQUIRE_FALSE(graph.insert_edge(node_a, node_c, 3.0000001));
		REQUIRE_FALSE(graph.insert_edge(node_b, node_d, -std::numeric_limits<double>::infinity()));
		REQUIRE_FALSE(graph.insert_edge(node_c, node_d, 3.000001));
		REQUIRE_FALSE(graph.insert_edge(node_d, node_a, 3.0000001));
	}

	SECTION("Verify Node and Edge Insertions") {
		REQUIRE(graph.is_node(node_a));
		REQUIRE(graph.is_node(node_d));
		REQUIRE(graph.is_connected(node_a, node_b));
		REQUIRE(graph.is_connected(node_a, node_c));
		REQUIRE_FALSE(graph.is_connected(node_a, node_d));
	}

	SECTION("Test Edge Erasure and Node Erasure") {
		REQUIRE(graph.erase_edge(node_a, node_b, 5.0));
		REQUIRE_FALSE(graph.is_connected(node_a, node_b));
		REQUIRE(graph.erase_node(node_d));
		REQUIRE_FALSE(graph.is_node(node_d));
	}

	auto constexpr original_output = std::string_view("\na (\n"
	                                                  // should be 5.0
	                                                  "  a -> b | W | 5\n"
	                                                  "  a -> c | W | -inf\n"
	                                                  // should be 3.000001
	                                                  "  a -> c | W | 3\n"
	                                                  // should be 3.0000001
	                                                  "  a -> c | W | 3\n"
	                                                  "  a -> c | W | inf\n"
	                                                  ")\n"
	                                                  "b (\n"
	                                                  "  b -> d | W | -inf\n"
	                                                  ")\n"
	                                                  "c (\n"
	                                                  // should be 3.000001
	                                                  "  c -> d | W | 3\n"
	                                                  ")\n"
	                                                  "d (\n"
	                                                  // should be 3.0000001
	                                                  "  d -> a | W | 3\n"
	                                                  ")\n");
	SECTION("Test Output of Graph") {
		auto oss = std::ostringstream{};
		oss << graph;
		REQUIRE(oss.str() == original_output);
	}

	SECTION("Test Graph Equality") {
		auto graph_copy = graph;
		REQUIRE(graph == graph_copy);
		graph_copy.insert_edge(node_a, node_d, 3.0);
		REQUIRE(graph != graph_copy);
	}

	SECTION("Replace Node and Check Edges") {
		auto const it = graph.find(node_a, node_b, 5.0);
		REQUIRE(it != graph.end());
		REQUIRE(it == graph.begin());
		auto const& [from, to, weight] = *it;
		REQUIRE(from == node_a);
		REQUIRE(to == node_b);
		REQUIRE(weight == 5.0);
		REQUIRE(graph.connections(node_a) == std::vector<std::string>{node_b, node_c});
		REQUIRE(std::next(it, 8) == graph.end());

		REQUIRE_THROWS_MATCHES(graph.replace_node(new_node, new_node),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node on a node that "
		                                                "doesn't exist"));
		REQUIRE_FALSE(graph.replace_node(node_b, node_a));

		REQUIRE(graph.replace_node(node_b, new_node));
		REQUIRE(graph.connections(node_a) == std::vector<std::string>{node_c, new_node});
		REQUIRE(graph.find(node_a, node_b, 5.0) == graph.end());

		REQUIRE_THROWS_MATCHES(graph.is_connected(node_a, node_b),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
		                                                "node don't exist in the graph"));
		REQUIRE(graph.is_connected(node_a, new_node));
		REQUIRE_THROWS_MATCHES(graph.connections(node_b),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if src doesn't "
		                                                "exist in the graph"));
	}

	SECTION("Merge Replace Node and Check Edges") {
		REQUIRE(graph.insert_edge(node_c, node_d, 20.0));
		REQUIRE(graph.insert_edge(node_b, node_c, 1.0));
		graph.merge_replace_node(node_b, node_c);
		REQUIRE(graph.is_connected(node_a, node_c));

		auto oss = std::ostringstream{};
		oss << graph;
		auto expected_output = std::string("\na (\n"
		                                   "  a -> c | W | -inf\n"
		                                   // 3.000001
		                                   "  a -> c | W | 3\n"
		                                   // 3.0000001
		                                   "  a -> c | W | 3\n"
		                                   // 5.0
		                                   "  a -> c | W | 5\n"
		                                   "  a -> c | W | inf\n"
		                                   ")\n"
		                                   "c (\n"
		                                   "  c -> c | W | 1\n"
		                                   "  c -> d | W | -inf\n"
		                                   // 3.000001
		                                   "  c -> d | W | 3\n"
		                                   "  c -> d | W | 20\n"
		                                   ")\n"
		                                   "d (\n"
		                                   // 3.0000001
		                                   "  d -> a | W | 3\n"
		                                   ")\n");
		REQUIRE(oss.str() == expected_output);

		REQUIRE_THROWS_MATCHES(graph.merge_replace_node(node_b, node_c),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
		REQUIRE_THROWS_MATCHES(graph.merge_replace_node(node_c, node_b),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                                                "new data if they don't exist in the graph"));
	}

	SECTION("Clear graph and Check Edge and Nodes Persistence") {
		auto const& edges = graph.edges(node_a, node_b);
		auto const& nodes = graph.nodes();
		graph.clear();
		REQUIRE(graph.empty());
		// Ensure edges vector still holds its data
		REQUIRE(edges.size() == 1);
		REQUIRE(nodes.size() == 4);
		REQUIRE(nodes == std::vector<std::string>{node_a, node_b, node_c, node_d});
	}

	SECTION("Erase Node") {
		REQUIRE(graph.erase_node(node_a));
		REQUIRE_FALSE(graph.erase_node(node_a));
		REQUIRE(graph.erase_node(node_b));
		REQUIRE(graph.erase_node(node_c));
		REQUIRE(graph.erase_node(node_d));
		REQUIRE(graph.empty());
	}

	SECTION("Erase Edge") {
		REQUIRE(graph.erase_edge(node_a, node_c, std::numeric_limits<double>::infinity()));
		REQUIRE_FALSE(graph.erase_edge(node_a, node_c, std::numeric_limits<double>::infinity()));
		REQUIRE_THROWS_MATCHES(graph.erase_edge(new_node, node_c),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
		                                                "they don't exist in the graph"));
		REQUIRE_FALSE(graph.erase_edge(node_a, node_b));
		REQUIRE(graph.erase_edge(node_a, node_b, 5.0));
	}

	SECTION("Erase Edge Iterator") {
		auto distance_bf = std::distance(graph.begin(), graph.end());
		auto it = graph.erase_edge(graph.begin());
		REQUIRE(it == graph.begin());
		REQUIRE_FALSE(std::distance(graph.begin(), graph.end()) == distance_bf);
		it = graph.erase_edge(std::next(it), std::prev(graph.end()));
		REQUIRE(it == std::prev(graph.end()));
		REQUIRE((it == std::next(graph.begin())));
		graph.erase_edge(graph.begin(), graph.end());
		REQUIRE(graph.begin() == graph.end());
	}

	SECTION("Test const method of copy constructor") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		// Should don't have a same iterator, but same value.
		REQUIRE(ng.begin() != graph.begin());
		auto const [ng_from, ng_to, ng_weight] = *ng.begin();
		auto const [from, to, weight] = *graph.begin();
		REQUIRE(ng_from == from);
		REQUIRE(ng_to == to);
		REQUIRE(ng_weight == weight);
		REQUIRE(std::distance(ng.begin(), ng.end()) == std::distance(graph.begin(), graph.end()));
		// Accessor should can running by const
		REQUIRE(ng.is_node(node_a) == graph.is_node(node_a));
		REQUIRE(ng.is_node(new_node) == graph.is_node(new_node));
		REQUIRE(ng.empty() == graph.empty());
		REQUIRE(ng.nodes() == graph.nodes());
		REQUIRE(ng.is_connected(node_a, node_b) == graph.is_connected(node_a, node_b));
		REQUIRE(ng.find(node_c, node_d) == ng.end());
		REQUIRE(ng.connections(node_a) == graph.connections(node_a));

		// But not edge because it is a vector of pointer.
		REQUIRE(ng.edges(node_a, node_b) != graph.edges(node_a, node_b));
	}

	SECTION("Const copy constructor and move constructor") {
		auto const copy = graph;
		auto const move = std::move(graph);
		REQUIRE(copy == move);
		auto out_copy = std::ostringstream{};
		out_copy << copy;
		auto out_move = std::ostringstream{};
		out_move << move;
		REQUIRE(out_copy.str() == out_move.str());
	}

	SECTION("Invalid Operations") {
		// Both nodes don't exist
		REQUIRE_THROWS_MATCHES(graph.edges("non_exist", "another"),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::edges if src or dst node don't "
		                                                "exist in the graph"));
		// 'e' does not exist
		REQUIRE_THROWS_MATCHES(graph.insert_edge("non_exist", "another", 20.0),
		                       std::runtime_error,
		                       Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                                                "dst node does not exist"));
		// Node 'e' does not exist
		REQUIRE_FALSE(graph.erase_node("another"));
	}

	SECTION("Erase after copy will not effect the new instance") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() == original_output);
		REQUIRE(graph.erase_node(node_a));
		oss.str("");
		oss.clear();
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
		REQUIRE(ng != graph);
	}

	SECTION("Replace after copy will not effect the new instance") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		REQUIRE(graph.replace_node(node_a, "New"));
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Replace merge after copy will not effect the new instance") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		graph.merge_replace_node(node_a, node_b);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Erase more than 1 edges not effect copy") {
		auto const ng = graph;
		graph.erase_edge(graph.begin());
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Insert node not effect the copy") {
		auto const ng = graph;
		graph.insert_node(new_node);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Insert edge not effect the copy") {
		auto const ng = graph;
		graph.insert_edge(node_a, node_b, 123.321);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Clear also not effect") {
		auto const ng = graph;
		graph.clear();
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Erase more than 1 edges not effect copy assignment") {
		auto ng = gdwg::graph<std::string, double>{};
		ng = graph;
		graph.erase_edge(graph.begin());
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Insert node not effect the copy assignment") {
		auto ng = gdwg::graph<std::string, double>{};
		ng = graph;
		graph.insert_node(new_node);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Insert edge not effect the copy assignment") {
		auto ng = gdwg::graph<std::string, double>{};
		ng = graph;
		graph.insert_edge(node_a, node_b, 123.321);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Clear also not effect copy assignment") {
		auto ng = gdwg::graph<std::string, double>{};
		ng = graph;
		graph.clear();
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		graph.erase_edge(graph.begin(), graph.end());
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Erase after copy assignment will not effect the new instance") {
		auto ng = gdwg::graph<std::string, double>{};
		ng = graph;
		REQUIRE(ng == graph);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() == original_output);
		REQUIRE(graph.erase_node(node_a));
		oss.str("");
		oss.clear();
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
		REQUIRE(ng != graph);
	}

	SECTION("Replace after copy assignment will not effect the new instance") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		REQUIRE(graph.replace_node(node_a, "New"));
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}

	SECTION("Replace merge after copy assignment will not effect the new instance") {
		auto const ng = graph;
		REQUIRE(ng == graph);
		graph.merge_replace_node(node_a, node_b);
		auto oss = std::ostringstream{};
		oss << ng;
		REQUIRE(oss.str() == original_output);
		oss.str("");
		oss.clear();
		oss << graph;
		REQUIRE(oss.str() != original_output);
	}
}
