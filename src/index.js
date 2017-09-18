const NDPSO = require('./js/NDPSO.js');
const {SimpleFacilityProblem} = require('./js/SimpleFacilityProblem.js');
const city1990 = require('./js/problems/city1990.json');

window.problem = new SimpleFacilityProblem(city1990);
window.ndpso = new NDPSO(problem);
console.time('city1990');
window.gBest = ndpso.optimize();
console.timeEnd('city1990');
