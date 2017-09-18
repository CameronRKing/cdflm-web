const test = require('tape');
const sinon = require('sinon');
const {SimpleFacilityProblem, OBJECTIVES, AGGREGATES, MEASURES, SIMPLE_COST_MATRIX} = require('../SimpleFacilityProblem.js');

test('new SimpleFacilityProblem defaults to minimize max star', function(t) {
    const p = new SimpleFacilityProblem();
    t.equal(p.problemName, 'minimize max star');
    t.end();
});

test('uses minimum comparator for minimization problems', function(t) {
    const p = new SimpleFacilityProblem();
    t.equal(p.objective, OBJECTIVES.MIN);

    const better = p.getBetterFitness(1, 2);
    t.equal(better, 1);
    t.end();
});

test('uses maximum comparator for maximization problems', function(t) {
    const p = new SimpleFacilityProblem();
    p.objective = OBJECTIVES.MAX;

    const better = p.getBetterFitness(1, 2);
    t.equal(better, 2);
    t.end();
});

test('assigns customer to closest facility and return [facility, cost]', function(t) {
    const p = new SimpleFacilityProblem();
    p.costs = SIMPLE_COST_MATRIX;
    t.true(p.objective = OBJECTIVES.MIN);
    t.looseEqual(p.assignCustomer(0, [0,4]), [0,0]);
    t.looseEqual(p.assignCustomer(1, [0,4]), [0,1]);
    t.looseEqual(p.assignCustomer(2, [0,4]), [0,2]);
    t.looseEqual(p.assignCustomer(3, [0,4]), [4,1]);
    t.looseEqual(p.assignCustomer(4, [0,4]), [4,0]);
    t.end();
});

test('can calculate stars for given facility vector and cost matrix', function(t) {
    const p = new SimpleFacilityProblem();
    p.costs = SIMPLE_COST_MATRIX;
    let stars = p._calcStars([0, 4]);
    t.looseEqual(stars, {
        0: 3,
        4: 1
    });
    t.end();
});

test('can calculate fitness for a given facility vector for a minimize max star problem', function(t) {
    const p = new SimpleFacilityProblem();
    p.costs = SIMPLE_COST_MATRIX;
    const fitness = p.calcFitness([0,4]);
    t.equal(fitness, 3);
    t.end();
});
