import {d4daglayout} from "./d4dag.js";

let graph, svg,
	graphData = {
		numLayer: 4,
		maxPerLayer: 2,
		nodeList: [
			{label: 'Rubecula', layer: 0},
			{label: 'Turdus', layer: 3},
			{label: 'Corvus', layer: 1},
			{label: 'Falco', layer: 1},
			{label: 'Cathartes', layer: 2},
			{label: 'Parus', layer: 0}
		],
		adjList: [
			[2, 3, 4],
			[],
			[1],
			[],
			[],
			[2]
		]
	};

graph = new d4daglayout({
});

console.log("Version number " + graph.version());

graph.layout(graphData);

