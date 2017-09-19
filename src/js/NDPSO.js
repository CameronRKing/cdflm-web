class NDPSO {
  constructor() {
    this.social = 0.7;
    this.cognitive = 0.7;
    this.inertia = 0.7;
    this.swarmSize = 100;
    this.maxIterations = 1000;
    this.swarm = [];
  }
  
  optimize(problem) {
    this.problem = problem;
    this.initSwarm();
    let uBest, gBest;
    uBest = gBest = this.getGlobalBest();
    for (let iter = 0; iter < this.maxIterations; iter++) {
      gBest = this.updateSwarm(gBest);
      if (this.problem.isBetter(gBest.fitness, uBest.fitness)) {
        uBest = gBest;
      }
    }
    return uBest;
  }

  updateSwarm(gBest) {
    this.swarm.forEach(p => p.update(gBest));
    return this.getGlobalBest();
  }

  initSwarm() {
    if (this.swarm.length) {
      this.swarm.splice(0, this.swarm.length);
    }
    for (let i = 0; i < this.swarmSize; i++) {
      this.swarm.push(this.createParticle());
    }
  }

  createParticle() {
    const p = {position: this.getRandomPosition()};
    p.fitness = this.problem.calcObjectiveFromFacilities(p.position);
    p.pBestPos = p.position;
    p.pBestFit = p.fitness;
    p.update = this.updateParticle.bind(this, p);
    return p;
  }
  
  getRandomPosition() {
    let position = [];
    for (let i = 0; i < this.problem.numFacilities; i++) {
        let fac;
        do {
            fac = Math.floor(Math.random() * this.problem.potentialFacs);
        } while (position.includes(fac));
        position.push(fac);
    }
    return position;
  }

  getGlobalBest() {
    return this.swarm.reduce((best, curr) => curr.fitness < best.fitness ? curr : best, {fitness: Number.POSITIVE_INFINITY});
  }

  /**
   * Returns a new array of facilities, possibly with one facility changed
   *
   * @param {Array} position - a list of facility numbers
   * @param {Number} chance - the chance (0-1) that the position will mutate
   **/
  mutatePosition(position, chance) {
    const newPos = position.slice(0);
    if (Math.random() >= chance) {
        // return a copy of the unchanged position
        return newPos;
    }

    // switch out a single facility
    const indexToChange = Math.floor(Math.random() * newPos.length)
    const oldFac = newPos[indexToChange];
    let newFac;
    do {
        newFac = Math.floor(Math.random() * (this.problem.numFacilities + 1));
    } while (position.includes(newFac) || newFac === oldFac);
    newPos[indexToChange] = newFac;
    return newPos;
  }

  updateParticle(particle, gBest) {
    const toMutate = [
      [gBest.position, this.social],
      [particle.pBestPos, this.cognitive],
      [particle.position, this.inertia]
    ];
    // potential is a hashtable of positions indexed by fitness
    // there is a small issue: if two positions have the same fitness,
    // then the later-mutated position will overwrite the previous one
    // doesn't cause any problems
    // (if two positions have the same fitness, doesn't matter which one we pick)
    // but worth noting anyway
    const potential = toMutate.reduce((obj, arr) => {
      let pos = this.mutatePosition(arr[0], arr[1]);
      let fit = this.problem.calcObjectiveFromFacilities(pos);
      obj[fit] = pos;
      return obj;
    }, {});

    // find best and assign to particle's current position/fitness
    // console.log(Object.keys(potential));
    particle.fitness = this.problem.getBestFitness(Object.keys(potential));
    particle.position = potential[particle.fitness];

    // update personal best
    if (this.problem.isBetter(particle.fitness, particle.pBestFit)) {
      particle.pBestPos = particle.position;
      particle.pBestFit = particle.fitness;
    }
  }
}

module.exports = NDPSO;