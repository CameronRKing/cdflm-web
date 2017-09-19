const NDPSO = require('./js/NDPSO.js');
const {SimpleFacilityProblem} = require('./js/SimpleFacilityProblem.js');
const city1990 = require('./js/problems/city1990.json');

window.ndpso = new NDPSO();
window.problem = new SimpleFacilityProblem(city1990);
console.time('city1990');
window.gBest = ndpso.optimize(problem);
console.timeEnd('city1990');
