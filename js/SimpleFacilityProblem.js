// Some enum objects + sample data
const OBJECTIVES = {
    MIN: 'minimize',
    MAX: 'maximize'
};
const AGGREGATES = {
    MIN: 'min',
    MAX: 'max',
    SUM: 'sum'
}

const MEASURES = {
    STAR: 'star',
    RADIUS: 'radius',
    RAY: 'ray'
}

const SIMPLE_COST_MATRIX = [
    [0,1,2,3,4],
    [1,0,2,3,3],
    [2,2,0,4,2],
    [3,3,4,0,1],
    [4,3,2,1,0]
];

// A class for representing simple facility allocation problems
// i.e., a combination max/min, an aggregate function, and a measure
// e.g., MINIMIZE MAX STAR or MAXIMIZE MIN RADIUS
class SimpleFacilityProblem {    
    constructor() {
        this.objective = OBJECTIVES.MIN;
        this.aggregate = AGGREGATES.MAX;
        this.measure = MEASURES.STAR;
        this.costs = SIMPLE_COST_MATRIX;
        this.numFacilities = 2;
    }

    get problemName() {
        return `${this.objective} ${this.aggregate} ${this.measure}`;
    }

    get potentialFacs() {
        return this.costs.length;
    }

    /**
     * Assigns customer to closest facility in facilities vector
     *
     * @param {number} cust - customer number (0-indexed)
     * @param {Array} facilities - array of potential facilities
     * @return {number} facility - the closest facility
     **/
    assignCustomer(cust, facilities) {
        let costRow = this.costs[cust];
        let bestVal, bestFac;
        let fac = facilities[0];
        bestVal = costRow[fac];
        bestFac = fac;
        for (let i = 1; i < facilities.length; i++) {
            fac = facilities[i];
            if (this.isBetter(costRow[fac], bestVal)) {
                bestVal = costRow[fac];
                bestFac = fac;
            }
        }
        return [bestFac, bestVal];
    }

    isBetter(left, right) {
        switch (this.objective) {
            case OBJECTIVES.MIN:
                return left < right;
            case OBJECTIVES.MAX:
                return right < left;
            default:
                throw Error('objective type (' + this.objective + ') not recongized!');
        }
    }

    getBetterFitness(left, right) {
        switch (this.objective) {
            case OBJECTIVES.MIN:
                return left < right ? left : right;
            case OBJECTIVES.MAX:
                return left > right ? left : right;
            default:
                throw Error('objective type (' + this.objective + ') not recognized!');
        }
    }

    getBestFitness(fitnessArray) {
        switch (this.objective) {
            case OBJECTIVES.MIN:
                return Math.min.apply(null, fitnessArray);
            case OBJECTIVES.MAX:
                return Math.max.apply(null, fitnessArray);
            default:
                throw Error('objective type(' + this.objective + ') not recognized!');
        }
    }

    getMeasures(facilities) {
        switch (this.measure) {
            case MEASURES.STAR:
                return this._calcStars(facilities);
            case MEASURES.RADIUS:
                return this._calcRadii(facilities);
            case MEASURES.RAY:
                return this._calcRays(facilities);
            default:
                throw Error('measure type (' + this.measure + ') not recognized!');
        }
    }

    getAggregate(measures) {
        // measures is an object keyed by facility number
        // pulling out the values makes them easier to manipulate,
        // especially since this function doesn't worry about 
        // returning a facility number with the aggregate val
        let values = Object.values(measures);
        switch (this.aggregate) {
            case AGGREGATES.MIN:
                return values.reduce((min, val) => val < min ? val : min, values[0]);
            case AGGREGATES.MAX:
                return values.reduce((max, val) => val > max ? val : max, values[0]);
            case AGGREGATES.SUM:
                return values.reduce((sum, val) => sum += val, 0);
        }
    }

    calcFitness(facilities) {
        let measures = this.getMeasures(facilities);
        return this.getAggregate(measures);
    }

    _calcStars(facilities) {
        let stars = facilities.reduce((stars, fac) => { 
            stars[fac] = 0;
            return stars;
        }, {});
        return this.costs.reduce((stars, costRow, cust) => {
            let [fac, cost] = this.assignCustomer(cust, facilities);
            stars[fac] += cost;
            return stars;
        }, stars);
    }

    _calcRadii(facilities) {
        return {};
    }

    _calcRays(facilities) {
        return {};
    }


}

module.exports = {SimpleFacilityProblem, OBJECTIVES, AGGREGATES, MEASURES, SIMPLE_COST_MATRIX};
