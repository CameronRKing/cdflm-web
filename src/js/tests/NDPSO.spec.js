const test = require('tape');
const sinon = require('sinon');
const NDPSO = require('../NDPSO.js');
const {SimpleFacilityProblem} = require('../SimpleFacilityProblem.js');

test('constructor initializes parameters to optimal defaults', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  t.equal(n.social, 0.7);
  t.equal(n.cognitive, 0.7);
  t.equal(n.inertia, 0.7);
  t.equal(n.inertialDiscount, 0.9995);
  t.equal(n.swarmSize, 100);
  t.equal(n.maxIterations, 1000);
  t.looseEqual(n.swarm, []);
  t.end();
});

test('getRandomPosition() returns an array with problem.numFacilities members each of which is a unique facility number from [0, potentialFacs-1]', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  // since the position is randomized, we should repeat the test several times to make sure passing isn't a fluke
  for (let test = 0; test < 5; test++) {
    const p = n.getRandomPosition();
    // make sure we have numFacilities entries
    t.equal(p.length, n.problem.numFacilities);
    // each facility is within [0, potentialFacs - 1]
    p.forEach(fac => t.true(0 <= fac && fac <= (n.problem.potentialFacs - 1)));
    // no two facilities are the same
    for (let i = 0; i < p.length - 1; i++) {
      for (let j = i + 1; j < p.length; j++) {
        t.notEqual(p[i], p[j]);
      }
    }    
  }
  t.end();
});

test('createParticle() initializes particle to random position, calcs fitness, and sets personal best', function(t) {
    const problem = new SimpleFacilityProblem();
    const n = new NDPSO(problem);
    const p = n.createParticle();
    // make sure p.position is a random array?
    t.equal(p.position.length, problem.numFacilities);
    t.equal(p.fitness, problem.calcFitness(p.position));
    t.equal(p.pBestPos, p.position);
    t.equal(p.pBestFit, p.fitness);
    t.end();
});

test('can initialize swarm', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  n.initSwarm();
  t.equal(n.swarm.length, n.swarmSize);
  t.end();
});

test('initSwarm() clears out this.swarm before adding new Particles', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  n.swarm = ['test!', 'not a real particle'];
  t.notEqual(n.swarm.length, n.swarmSize);
  n.initSwarm();
  t.equal(n.swarm.length, n.swarmSize);
  t.end();
});

test('getGlobalBest() returns particle with lowest fitness in swarm', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  n.swarm = [
    {fitness: 1, id: 1},
    {fitness: 2, id: 2}
  ];
  let particle = n.getGlobalBest();
  t.looseEqual(particle, {fitness: 1, id: 1});
  t.end();
});

test('updateSwarm() accepts globalBest, calls particle.update(globalBest) for each particle in the swarm, and returns new globalBest', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  const p1 = {update: sinon.spy(), fitness: 1};
  const p2 = {update: sinon.spy(), fitness: 2};
  const p3 = {update: sinon.spy(), fitness: 3};
  n.swarm = [p1, p2, p3];

  let newBest = n.updateSwarm(p2);
  t.equal(newBest, p1);
  t.true(p1.update.calledWith(p2));
  t.true(p2.update.calledWith(p2));
  t.true(p3.update.calledWith(p2));
  t.end();
});

test('optimize() calls initSwarm and updates the swarm maxIterations times', function(t) {
  const n = new NDPSO(new SimpleFacilityProblem());
  const initSwarm = sinon.spy(n, 'initSwarm');
  const updateSwarm = sinon.spy(n, 'updateSwarm');

  n.maxIterations = 100;
  n.optimize();

  t.true(initSwarm.calledOnce);
  t.true(updateSwarm.callCount, n.maxIterations);
  t.end();
});

test('mutatePosition() returns a copy of the passed-in position if the mutate doesnt fire', function(t) {
    const n = new NDPSO(new SimpleFacilityProblem());

    const oldPos = [1,2,3];
    const newPos = n.mutatePosition(oldPos, 0);
    // make sure they contain the same values but are not the same object
    t.looseEqual(newPos, oldPos);
    t.notEqual(newPos, oldPos);
    t.end();
});

test('mutatePosition() returns a copy of the position with a single facility changed if it fires', function(t) {
    const n = new NDPSO(new SimpleFacilityProblem());

    const oldPos = [1,2,3];
    const newPos = n.mutatePosition(oldPos, 1);

    // neither the same object nor the same values
    t.notEqual(newPos, oldPos);
    t.notLooseEqual(newPos, oldPos);
    t.equal(newPos.length, oldPos.length);
    // but only one value should be different
    let sameVals = newPos.reduce((numMatches, val) => oldPos.includes(val) ? numMatches + 1 : numMatches, 0);
    t.equal(sameVals, 2);
    if (sameVals === 3) {
      console.log(oldPos, newPos);
    }
    t.end();
});

test('particle.update() calls mutatePosition with the appropriate position/parameter combinations', function(t) {
    const n = new NDPSO(new SimpleFacilityProblem());
    const gBest = n.createParticle();
    const p = n.createParticle();
    const oldPos = p.position;
    const oldBest = p.pBestPos;
    const mutatePosition = sinon.spy(n, 'mutatePosition');

    p.update(gBest);
    
    // s1 = current position MUTATE inertia
    t.true(mutatePosition.calledWith(oldPos, n.inertia));
    // s2 = pBestPos MUTATE cognitive
    t.true(mutatePosition.calledWith(oldBest, n.cognitive));
    // s3 = gBest.position MUTATE social
    t.true(mutatePosition.calledWith(gBest.position, n.social));

    // new position = best(s1, s2, s3)
    let positions = mutatePosition.returnValues;
    let fitnesses = positions.map(p => n.problem.calcFitness(p));
    let bestFit = n.problem.getBestFitness(fitnesses);
    // have to find lastIndexOf because of how updateParticle() chooses
    // among positions with the same fitness (last calculated has priority)
    let bestPos = positions[fitnesses.lastIndexOf(bestFit)];
    t.equal(bestFit, p.fitness);
    t.notEqual(p.position, p.oldPos);
    // use looseEqual since position is an array and the objects themselves should not be equivalent -- just their values
    t.looseEqual(bestPos.sort(), p.position.sort());

    t.end();
});
