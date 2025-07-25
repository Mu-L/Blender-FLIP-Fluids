/*
MIT License

Copyright (C) 2025 Ryan L. Guy & Dennis Fassbaender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef FLUIDENGINE_DIFFUSEPARTICLESIMULATION_H
#define FLUIDENGINE_DIFFUSEPARTICLESIMULATION_H

#if __MINGW32__ && !_WIN64
    #include "mingw32_threads/mingw.thread.h"
#else
    #include <thread>
#endif

#include "vmath.h"
#include "fragmentedvector.h"
#include "array3d.h"
#include "aabb.h"
#include "fluidmaterialgrid.h"
#include "turbulencefield.h"
#include "particlesystem.h"
#include "diffuseparticle.h"

struct MarkerParticle;
enum class DiffuseParticleType : char;
class MeshLevelSet;
class MACVelocityField;
class ParticleLevelSet;
class ForceFieldGrid;

struct DiffuseParticleSimulationParameters {
    int isize;
    int jsize;
    int ksize;
    double dx;
    double deltaTime;
    double CFLConditionNumber;
    double markerParticleRadius;
    vmath::vec3 bodyForce;

    ParticleSystem *markerParticles;
    MACVelocityField *vfield;
    ParticleLevelSet *liquidSDF;
    MeshLevelSet *solidSDF;
    Array3d<float> *surfaceSDF;
    MeshLevelSet *meshingVolumeSDF;
    bool isMeshingVolumeSet = false;
    Array3d<float> *curvatureGrid;
    Array3d<float> *influenceGrid;
    Array3d<bool> *nearSolidGrid;
    double nearSolidGridCellSize;
    ForceFieldGrid *forceFieldGrid;
    bool isForceFieldGridSet = false;
};

enum class LimitBehaviour : char { 
    collide   = 0x00, 
    ballistic = 0x01, 
    kill = 0x02
};

class DiffuseParticleSimulation
{

public:
    DiffuseParticleSimulation();
    ~DiffuseParticleSimulation();

    void update(DiffuseParticleSimulationParameters params);

    void getDiffuseParticleTypeCounts(int *numfoam, 
                                    int *numbubble, 
                                    int *numspray,
                                    int *numdust);
    int getNumSprayParticles();
    int getNumBubbleParticles();
    int getNumFoamParticles();
    int getNumDustParticles();

    void enableDiffuseParticleEmission();
    void disableDiffuseParticleEmission();
    bool isDiffuseParticleEmissionEnabled();

    void enableFoam();
    void disableFoam();
    bool isFoamEnabled();

    void enableBubbles();
    void disableBubbles();
    bool isBubblesEnabled();

    void enableSpray();
    void disableSpray();
    bool isSprayEnabled();

    void enableDust();
    void disableDust();
    bool isDustEnabled();

    void enableBoundaryDustEmission();
    void disableBoundaryDustEmission();
    bool isBoundaryDustEmissionEnabled();

    ParticleSystem* getDiffuseParticles();
    int getNumDiffuseParticles();

    double getForceFieldWeightWhitewaterFoam();
    void setForceFieldWeightWhitewaterFoam(double v);
    double getForceFieldWeightWhitewaterBubble();
    void setForceFieldWeightWhitewaterBubble(double v);
    double getForceFieldWeightWhitewaterSpray();
    void setForceFieldWeightWhitewaterSpray(double v);
    double getForceFieldWeightWhitewaterDust();
    void setForceFieldWeightWhitewaterDust(double v);

    double getEmitterGenerationRate();
    void setEmitterGenerationRate(double rate);

    double getMinEmitterEnergy();
    void setMinEmitterEnergy(double e);
    double getMaxEmitterEnergy();
    void setMaxEmitterEnergy(double e);

    double getMinWavecrestCurvature();
    void setMinWavecrestCurvature(double k);
    double getMaxWavecrestCurvature();
    void setMaxWavecrestCurvature(double k);

    double getMinTurbulence();
    void setMinTurbulence(double t);
    double getMaxTurbulence();
    void setMaxTurbulence(double t);

    int getMaxNumDiffuseParticles();
    void setMaxNumDiffuseParticles(int n);
    AABB getEmitterGenerationBounds();
    void setEmitterGenerationBounds(AABB bbox);

    double getMinDiffuseParticleLifetime();
    void setMinDiffuseParticleLifetime(double lifetime);
    double getMaxDiffuseParticleLifetime();
    void setMaxDiffuseParticleLifetime(double lifetime);
    double getDiffuseParticleLifetimeVariance();
    void setDiffuseParticleLifetimeVariance(double variance);
    double getFoamParticleLifetimeModifier();
    void setFoamParticleLifetimeModifier(double modifier);
    double getBubbleParticleLifetimeModifier();
    void setBubbleParticleLifetimeModifier(double modifier);
    double getSprayParticleLifetimeModifier();
    void setSprayParticleLifetimeModifier(double modifier);
    double getDustParticleLifetimeModifier();
    void setDustParticleLifetimeModifier(double modifier);

    double getDiffuseParticleWavecrestEmissionRate();
    void setDiffuseParticleWavecrestEmissionRate(double r);

    double getDiffuseParticleTurbulenceEmissionRate();
    void setDiffuseParticleTurbulenceEmissionRate(double r);

    double getDiffuseParticleDustEmissionRate();
    void setDiffuseParticleDustEmissionRate(double r);

    double getFoamAdvectionStrength();
    void setFoamAdvectionStrength(double s);
    double getFoamLayerDepth();
    void setFoamLayerDepth(double depth);
    double getFoamLayerOffset();
    void setFoamLayerOffset(double offset);

    void enablePreserveFoam();
    void disablePreserveFoam();
    bool isPreserveFoamEnabled();
    double getFoamPreservationRate();
    void setFoamPreservationRate(double rate);
    double getMinFoamDensity();
    void setMinFoamDensity(double d);
    double getMaxFoamDensity();
    void setMaxFoamDensity(double d);

    double getBubbleDragCoefficient();
    void setBubbleDragCoefficient(double d);
    double getBubbleBouyancyCoefficient();
    void setBubbleBouyancyCoefficient(double b);

    double getDustDragCoefficient();
    void setDustDragCoefficient(double d);
    double getDustBouyancyCoefficient();
    void setDustBouyancyCoefficient(double b);

    double getSprayDragCoefficient();
    void setSprayDragCoefficient(double d);
    double getSprayEmissionSpeed();
    void setSprayEmissionSpeed(double value);

    LimitBehaviour getFoamLimitBehaviour();
    void setFoamLimitBehavour(LimitBehaviour b);

    LimitBehaviour getBubbleLimitBehaviour();
    void setBubbleLimitBehavour(LimitBehaviour b);

    LimitBehaviour getSprayLimitBehaviour();
    void setSprayLimitBehavour(LimitBehaviour b);

    LimitBehaviour getDustLimitBehaviour();
    void setDustLimitBehavour(LimitBehaviour b);

    std::vector<bool> getFoamActiveBoundarySides();
    void setFoamActiveBoundarySides(std::vector<bool> active);

    std::vector<bool> getBubbleActiveBoundarySides();
    void setBubbleActiveBoundarySides(std::vector<bool> active);

    std::vector<bool> getSprayActiveBoundarySides();
    void setSprayActiveBoundarySides(std::vector<bool> active);

    std::vector<bool> getDustActiveBoundarySides();
    void setDustActiveBoundarySides(std::vector<bool> active);

    std::vector<bool> getFoamBoundaryCollisions();
    void setFoamBoundaryCollisions(std::vector<bool> active);

    std::vector<bool> getBubbleBoundaryCollisions();
    void setBubbleBoundaryCollisions(std::vector<bool> active);

    std::vector<bool> getSprayBoundaryCollisions();
    void setSprayBoundaryCollisions(std::vector<bool> active);

    std::vector<bool> getDustBoundaryCollisions();
    void setDustBoundaryCollisions(std::vector<bool> active);

    void setDiffuseOpenBoundaryWidth(int width);

    void setDomainOffset(vmath::vec3 offset);
    vmath::vec3 getDomainOffset();
    void setDomainScale(double scale);
    double getDomainScale();
    void getDiffuseParticleFileDataWWP(std::vector<char> &data);
    void getFoamParticleFileDataWWP(std::vector<char> &data);
    void getBubbleParticleFileDataWWP(std::vector<char> &data);
    void getSprayParticleFileDataWWP(std::vector<char> &data);
    void getDustParticleFileDataWWP(std::vector<char> &data);
    void getFoamParticleBlurFileDataWWP(std::vector<char> &data, double dt);
    void getBubbleParticleBlurFileDataWWP(std::vector<char> &data, double dt);
    void getSprayParticleBlurFileDataWWP(std::vector<char> &data, double dt);
    void getDustParticleBlurFileDataWWP(std::vector<char> &data, double dt);
    void getFoamParticleVelocityAttributeFileDataWWP(std::vector<char> &data);
    void getBubbleParticleVelocityAttributeFileDataWWP(std::vector<char> &data);
    void getSprayParticleVelocityAttributeFileDataWWP(std::vector<char> &data);
    void getDustParticleVelocityAttributeFileDataWWP(std::vector<char> &data);
    void getFoamParticleIDAttributeFileDataWWI(std::vector<char> &data);
    void getBubbleParticleIDAttributeFileDataWWI(std::vector<char> &data);
    void getSprayParticleIDAttributeFileDataWWI(std::vector<char> &data);
    void getDustParticleIDAttributeFileDataWWI(std::vector<char> &data);
    void getFoamParticleLifetimeAttributeFileDataWWF(std::vector<char> &data);
    void getBubbleParticleLifetimeAttributeFileDataWWF(std::vector<char> &data);
    void getSprayParticleLifetimeAttributeFileDataWWF(std::vector<char> &data);
    void getDustParticleLifetimeAttributeFileDataWWF(std::vector<char> &data);

    void loadDiffuseParticles(FragmentedVector<DiffuseParticle> &particles);

private:

    struct DiffuseParticleEmitter {
        vmath::vec3 position;
        vmath::vec3 velocity;
        double energyPotential;
        double wavecrestPotential;
        double turbulencePotential;
        double dustPotential;

        DiffuseParticleEmitter() : energyPotential(0.0),
                                   wavecrestPotential(0.0),
                                   turbulencePotential(0.0),
                                   dustPotential(0.0) {}

        DiffuseParticleEmitter(vmath::vec3 p, vmath::vec3 v, 
                               double e, double wc, double t, double d) : 
                                   position(p),
                                   velocity(v),
                                   energyPotential(e),
                                   wavecrestPotential(wc),
                                   turbulencePotential(t),
                                   dustPotential(d) {}
    };    

    struct DiffuseParticleAttributes {
        std::vector<vmath::vec3> *positions = nullptr;
        std::vector<vmath::vec3> *velocities = nullptr;
        std::vector<float> *lifetimes = nullptr;
        std::vector<char> *types = nullptr;
        std::vector<unsigned char> *ids = nullptr;

        DiffuseParticle getDiffuseParticle(size_t index) {
            DiffuseParticle dp;
            dp.position = positions->at(index);
            dp.velocity = velocities->at(index);
            dp.lifetime = lifetimes->at(index);
            dp.type = (DiffuseParticleType)types->at(index);
            dp.id = ids->at(index);
            return dp;
        }
    };

    DiffuseParticleAttributes _getDiffuseParticleAttributes();

    void _trilinearInterpolate(std::vector<vmath::vec3> &input, MACVelocityField *vfield, 
                               std::vector<vmath::vec3> &output);
    void _trilinearInterpolateThread(int startidx, int endidx, 
                                     std::vector<vmath::vec3> *input, MACVelocityField *vfield, 
                                     std::vector<vmath::vec3> *output);
    void _getDiffuseParticleEmitters(std::vector<DiffuseParticleEmitter> &normalEmitters,
                                     std::vector<DiffuseParticleEmitter> &dustEmitters);
    void _sortMarkerParticlePositions(std::vector<vmath::vec3> &surface, 
                                      std::vector<vmath::vec3> &inside);
    double _getParticleJitter();
    vmath::vec3 _jitterParticlePosition(vmath::vec3 p, double jitter);
    void _initializeMaterialGrid();
    void _initializeMaterialGridThread(int startidx, int endidx);
    void _shrinkMaterialGridFluidThread(int startidx, int endidx, 
                                        FluidMaterialGrid *mgridtemp);
    void _getSurfaceDiffuseParticleEmitters(std::vector<vmath::vec3> &surface, 
                                            std::vector<DiffuseParticleEmitter> &emitters);
    double _getWavecrestPotential(vmath::vec3 p, vmath::vec3 v);
    double _getTurbulencePotential(vmath::vec3 p, TurbulenceField &tfield);
    double _getDustTurbulencePotential(vmath::vec3 p, double emissionStrength, TurbulenceField &tfield);
    double _getEnergyPotential(vmath::vec3 velocity);
    void _getInsideDiffuseParticleEmitters(std::vector<vmath::vec3> &inside, 
                                           std::vector<DiffuseParticleEmitter> &emitters);
    void _getDiffuseDustParticleEmitters(std::vector<vmath::vec3> &particles, 
                                         std::vector<DiffuseParticleEmitter> &dustEmitters);
    void _shuffleDiffuseParticleEmitters(std::vector<DiffuseParticleEmitter> &emitters);

    void _addNewDiffuseParticles(std::vector<DiffuseParticle> &newDiffuseParticles);
    void _emitNormalDiffuseParticles(std::vector<DiffuseParticleEmitter> &emitters, double dt);
    void _emitDustDiffuseParticles(std::vector<DiffuseParticleEmitter> &emitters, double dt);
    void _emitDiffuseParticles(DiffuseParticleEmitter &emitter, 
                               double dt,
                               std::vector<DiffuseParticle> &particles);
    int _getNumberOfEmissionParticles(DiffuseParticleEmitter &emitter,
                                      double dt);
    unsigned char _getDiffuseParticleID();
    void _computeNewDiffuseParticleVelocities(std::vector<DiffuseParticle> &particles);

    void _updateDiffuseParticleTypes();
    DiffuseParticleType _getDiffuseParticleType(DiffuseParticle &p, AABB &boundary);

    void _updateDiffuseParticleLifetimes(double dt);
    void _updateFoamPreservation(double dt);

    void _advanceDiffuseParticles(double dt);
    AABB _getBoundaryAABB();
    void _advanceSprayParticles(double dt);
    void _advanceBubbleParticles(double dt);
    void _advanceFoamParticles(double dt);
    void _advanceDustParticles(double dt);
    void _advanceSprayParticlesThread(int startidx, int endidx, double dt);
    void _advanceBubbleParticlesThread(int startidx, int endidx, double dt);
    void _advanceFoamParticlesThread(int startidx, int endidx, double dt);
    void _advanceDustParticlesThread(int startidx, int endidx, double dt);
    vmath::vec3 _resolveCollision(vmath::vec3 oldp, vmath::vec3 newp, 
                                  DiffuseParticle &dp, AABB &boundary);
    bool _resolveSprayCollision(vmath::vec3 oldp, vmath::vec3 newp,
                                DiffuseParticle &dp, AABB &boundary,
                                vmath::vec3 &nextp, vmath::vec3 &nextv);
    LimitBehaviour _getLimitBehaviour(DiffuseParticle &dp);
    std::vector<bool>* _getActiveSides(DiffuseParticle &dp);
    int _getNearestSideIndex(vmath::vec3 p, AABB &boundary);

    vmath::vec3 _getGravityVector(vmath::vec3 pos, DiffuseParticleType dtype);

    void _getDiffuseParticleTypeCounts(int *numfoam, 
                                      int *numbubble, 
                                      int *numspray,
                                      int *numdust);
    int _getNumSprayParticles();
    int _getNumBubbleParticles();
    int _getNumFoamParticles();
    int _getNumDustParticles();

    void _removeDiffuseParticles();

    void _getDiffuseParticleFileDataWWP(std::vector<vmath::vec3> &positions, 
                                        std::vector<unsigned char> &ids,
                                        std::vector<char> &data);
    void _getDiffuseParticleFileDataWWI(std::vector<int> &intvalues, 
                                        std::vector<unsigned char> &ids,
                                        std::vector<char> &data);
    void _getDiffuseParticleFileDataWWF(std::vector<float> &floatvalues, 
                                        std::vector<unsigned char> &ids,
                                        std::vector<char> &data);

    template<class T>
    void _removeItemsFromVector(FragmentedVector<T> &items, std::vector<bool> &isRemoved) {
        FLUIDSIM_ASSERT(items.size() == isRemoved.size());

        int currentidx = 0;
        for (unsigned int i = 0; i < items.size(); i++) {
            if (!isRemoved[i]) {
                items[currentidx] = items[i];
                currentidx++;
            }
        }

        int numRemoved = (int)items.size() - currentidx;
        for (int i = 0; i < numRemoved; i++) {
            items.pop_back();
        }
        items.shrink_to_fit();
    }

    inline double _randomDouble(double min, double max) {
        return min + ((double)rand() / (double)RAND_MAX) * (max - min);
    }

    int _isize = 0;
    int _jsize = 0;
    int _ksize = 0;
    double _dx;
    double _CFLConditionNumber = 5;
    double _markerParticleRadius = 0;
    vmath::vec3 _bodyForce;
    float _forceFieldWeightWhitewaterFoam = 1.0f;
    float _forceFieldWeightWhitewaterBubble = 1.0f;
    float _forceFieldWeightWhitewaterSpray = 1.0f;
    float _forceFieldWeightWhitewaterDust = 1.0f;
    vmath::vec3 _domainOffset;
    double _domainScale = 1.0;

    bool _isDiffuseParticleEmissionEnabled = true;
    bool _isFoamEnabled = true;
    bool _isBubblesEnabled = true;
    bool _isSprayEnabled = true;
    bool _isDustEnabled = false;
    bool _isBoundaryDustEmissionEnabled = false;
    double _diffuseSurfaceNarrowBandSize = 1.5;  // in number of grid cells
    double _solidBufferWidth = 0.25;             // in number of grid cells
    double _maxVelocityFactor = 1.1;
    double _wavecrestSharpness = 0.4;
    double _minWavecrestCurvature = 0.4;
    double _maxWavecrestCurvature = 1.0;
    double _minParticleEnergy = 0.1;
    double _maxParticleEnergy = 60.0;
    double _minTurbulence = 100.0;
    double _maxTurbulence = 200.0;
    double _minDustTurbulenceFactor = 0.75;
    double _maxDustTurbulenceFactor = 1.0;
    double _emitterGenerationRate = 1.0;
    unsigned int _maxNumDiffuseParticles = 10e6;
    unsigned int _maxNumDiffuseParticlesLimit = 357e6;
    double _minDiffuseParticleLifetime = 0.0;
    double _maxDiffuseParticleLifetime = 7.0;
    double _lifetimeVariance = 3.0;
    double _wavecrestEmissionRate = 175;
    double _turbulenceEmissionRate = 175;
    double _dustEmissionRate = 175;
    double _foamLayerOffset = 0.0;                     // in number of grid cells
    double _maxFoamToSurfaceDistance = 1.0;            // in number of grid cells
    double _foamBufferWidth = 1.0;                     // in number of grid cells
    double _maxDustEmitterToObstacleDistance = 2.5;    // in number of grid cells
    double _sprayParticleLifetimeModifier = 2.0;
    double _bubbleParticleLifetimeModifier = 0.333;
    double _foamParticleLifetimeModifier = 1.0;
    double _dustParticleLifetimeModifier = 1.0;
    double _foamAdvectionStrength = 1.0;
    double _bubbleBouyancyCoefficient = 4.0;
    double _bubbleDragCoefficient = 1.0;
    double _dustBouyancyCoefficient = -4.0;
    double _dustBouyancyVarianceFactor = 0.5;
    double _dustDragCoefficient = 0.5;
    double _dustDragVarianceFactor = 0.25;
    double _sprayDragCoefficient = 0.0;
    double _sprayDragVarianceFactor = 0.25;
    double _sprayEmissionSpeedFactor = 1.0;
    double _sprayCollisionFriction = 0.0;
    double _sprayCollisionRestitution = 0.2;
    double _maxDiffuseParticlesPerCell = 5000;
    double _emitterRadiusFactor = 8.0;            // in multiples of _markerParticleRadius
    double _particleJitterFactor = 1.0;
    double _diffuseParticleStepDistanceFactor = 0.5;

    bool _isPreserveFoamEnabled = false;
    double _foamPreservationRate = 0.75;
    double _minFoamDensity = 20;
    double _maxFoamDensity = 45;

    LimitBehaviour _foamLimitBehaviour = LimitBehaviour::collide;
    LimitBehaviour _bubbleLimitBehaviour = LimitBehaviour::collide;
    LimitBehaviour _sprayLimitBehaviour = LimitBehaviour::collide;
    LimitBehaviour _dustLimitBehaviour = LimitBehaviour::collide;
    std::vector<bool> _foamActiveSides{true, true, true, true, true, true};
    std::vector<bool> _bubbleActiveSides{true, true, true, true, true, true};
    std::vector<bool> _sprayActiveSides{true, true, true, true, true, true};
    std::vector<bool> _dustActiveSides{true, true, true, true, true, true};
    AABB _emitterGenerationBounds;

    std::vector<bool> _foamBoundaryCollisions{true, true, true, true, true, true};
    std::vector<bool> _bubbleBoundaryCollisions{true, true, true, true, true, true};
    std::vector<bool> _sprayBoundaryCollisions{true, true, true, true, true, true};
    std::vector<bool> _dustBoundaryCollisions{true, true, true, true, true, true};
    int _openBoundaryWidth = 2;    // in # of voxels

    ParticleSystem *_markerParticles;
    MACVelocityField *_vfield;
    ParticleLevelSet *_liquidSDF;
    MeshLevelSet *_solidSDF;
    Array3d<float> *_surfaceSDF;
    MeshLevelSet *_meshingVolumeSDF = NULL;
    bool _isMeshingVolumeSet = false;
    Array3d<float> *_kgrid;
    Array3d<float> *_influenceGrid;
    Array3d<bool> *_nearSolidGrid;
    double _nearSolidGridCellSize = 0.0;
    ForceFieldGrid *_forceFieldGrid;
    bool _isForceFieldGridSet = false;

    FluidMaterialGrid _mgrid;
    Array3d<bool> _borderingAirGrid;
    Array3d<bool> _isBorderingAirGridSet;
    TurbulenceField _turbulenceField;
    ParticleSystem _diffuseParticles;

    int _currentDiffuseParticleID = 0;
    int _diffuseParticleIDLimit = 256;
};

#endif