/*
 * Thermal-FIST package
 * 
 * Copyright (c) 2014-2019 Volodymyr Vovchenko
 *
 * GNU General Public License (GPLv3 or later)
 */
#ifndef THERMALPARTICLESYSTEM_H
#define THERMALPARTICLESYSTEM_H

#include <map>
#include <vector>
#include <fstream>

#include "HRGBase/ThermalParticle.h"

namespace thermalfist {

  /**
   * \brief Class containing the particle list.
   * 
   * Class containing the information about
   * the particle list.
   * The list is represented by a vector of ThermalParticle objects
   * and is usually read from a file.
   * 
   */
  class ThermalParticleSystem
  {
  public:
    /**
     * \brief Construct a new ThermalParticleSystem object
     * 
     * The list is read from the specified file with LoadTable() method.
     * 
     * \param InputFile Path to the file with particle list.
     * \param GenAntiP  Whether antiparticles are to be generated from particles automatically.
     * \param mcut      Discard particles with mass > mcut (in GeV).
     */
    ThermalParticleSystem(std::string InputFile = "", bool GenAntiP = true, double mcut = 1.e9);

    /**
     * \brief Destroy the ThermalParticleSystem object
     * 
     */
    ~ThermalParticleSystem(void);

    /**
     * \brief Generates the decay channels for an antiparticle
     *        based on the provided decay channels of a particle.
     * 
     * \param Decays The decay channels of a particle.
     * \return std::vector<ParticleDecay> The generated  decay channels of an antiparticle.
     */
    ThermalParticle::ParticleDecaysVector GetDecaysFromAntiParticle(const ThermalParticle::ParticleDecaysVector& Decays);

    /**
     * \brief Computes the decay contributions of decaying resonances
     *        to all particle yields.
     * 
     */
    void ProcessDecays();

    /**
     * \brief Computes and fills decay channels of all particles
     *        with extra information.
     * 
     * Computes mass thresholds, released angular momentum for
     * all decay channels of all particles. 
     * Computes and sets the coefficients for mass integration
     * in the eBW scheme.
     * 
     */
    void FillDecayProperties();
    
    /**
     * \brief Computes mass thresholds of all decay channels of all particles. Obsolete.
     * 
     */
    void FillDecayThresholds();

    /**
     * \brief Computes the decay contributions of decaying resonances
     *        to all particle yields. 
     * 
     * Feeddown only according to stability flags is considered here.
     * 
     */
    void FillResonanceDecays();

    /**
     * \brief Same as FillResonanceDecays() but separately for
     *        weak, electromagnetic, and strong decay feeddowns.
     * 
     */
    void FillResonanceDecaysByFeeddown();


    /// Construction to hold mean number of certain species which results from a decay of a certain resonance.  
    ///
    /// The First element is the mean number of particles and the second element is the 0-based index of a resonance.
    typedef std::pair<double, int> SingleDecayContribution;

    /// A vector of SingleDecayContribution where each element corresponds to a certain resonance species.
    typedef std::vector<SingleDecayContribution> DecayContributionsToParticle;

    /// A vector of DecayContributionsToParticle where each
    /// element corresponds to a certain particle species resulting from decay.
    typedef std::vector<DecayContributionsToParticle> DecayContributionsToAllParticles;

    /// Holds cumulants of the particle number distribution of certain species which appear in a decay of a certain resonance.  
    ///
    /// The First element contains the leading four cumulants and the second element is the 0-based index of a resonance.
    typedef std::pair< std::vector<double>, int> SingleDecayCumulantsContribution;

    /// A vector of SingleDecayCumulantsContribution where each element corresponds to a certain resonance species.
    typedef std::vector<SingleDecayCumulantsContribution> DecayCumulantsContributionsToParticle;

    /// A vector of DecayCumulantsContributionsToParticle where each
    /// element corresponds to a certain particle species resulting from decay.
    typedef std::vector<DecayCumulantsContributionsToParticle> DecayCumulantsContributionsToAllParticles;

    /// Construction to hold the probability distribution of the number of
    /// certain species which appear in a decay of a certain resonance.  
    ///
    /// The first element contains the probability distribution
    /// and the second element is the 0-based index of a resonance.
    typedef std::pair< std::vector<double>, int> SingleDecayProbabilityDistribution;

    /// A vector of SingleDecayProbabilityDistribution where each element corresponds to a certain resonance species.
    typedef std::vector<SingleDecayProbabilityDistribution> DecayProbabilityDistributionsToParticle;

    /// A vector of DecayProbabilityDistributionsToParticle where each
    /// element corresponds to a certain particle species resulting from decay.
    typedef std::vector<DecayProbabilityDistributionsToParticle> DecayProbabilityDistributionsToAllParticles;

    /// A vector of final state particle number distributions for the final state (after decays)
    /// of each particle species.  
    /// One element per each particle species in the list.
    /// Each element is a std::pair where the first element the probability of a specific final state
    /// and the second element contains the numbers of all particle species in this final state 
    typedef std::vector< std::pair<double, std::vector<int> > > ResonanceFinalStatesDistribution;

    /**
     *   Returns information about decay chains of heavier particles including strong, strong/electromagnetic, or strong/electromagnetic/weak decay feeddown 
     *   resulting in production of of all the particles.
     *   
     *   Each element corresponds to a specific Feeddown::Type.
     */
    const std::vector<DecayContributionsToAllParticles>& DecayContributionsByFeeddown() const { return m_DecayContributionsByFeeddown; }

    /**
     * \brief Cumulants of particle number distributions of from decays.
     * 
     * Used to calculate effects of probabilistic decays on the final state
     * fluctuations.
     * 
     * \return const DecayCumulantsContributionsToAllParticles& 
     */
    const DecayCumulantsContributionsToAllParticles& DecayCumulants() const { return m_DecayCumulants; }

    /**
     * \brief Final state particle number distributions for resonance decays.
     * 
     * \return const std::vector<ResonanceFinalStatesDistribution>& -- a vector of final state distributions, 
     *         one element per each resonance.
     */
    const std::vector<ResonanceFinalStatesDistribution>& ResonanceFinalStatesDistributions() const { return m_ResonanceFinalStatesDistributions; }

    /**
     * \brief Loads the particle list from file.
     * 
     * The list is read from the specified file.
     * Decays are read from file "decays.dat" from
     * the same directory as the input list.
     * This can be overriden by ReadDecays() method.
     * 
     * \param InputFile Path to the file with particle list.
     * \param GenAntiP  Whether antiparticles are to be generated from particles automatically.
     * \param mcut      Discard particles with mass > mcut (in GeV).
     */
    void LoadList(std::string InputFile = "", bool GenerateAntiParticles = true, double mcut = 1.e9);

    /**
     * \brief Same as LoadList()
     *
     * \deprecated
     */
    void LoadTable(std::string InputFile = "", bool GenerateAntiParticles = true, double mcut = 1.e9) { LoadList(InputFile, GenerateAntiParticles, mcut); }
    
    /**
     * \brief Sets the particle list from a
     *        provided vector of ThermalParticle objects.
     * 
     * \param part_in Input vector of ThermalParticle objects.
     * \param GenerateAntiParticles  Whether antiparticles are to be generated from particles automatically.
     */
    void SetTableFromVector(const std::vector<ThermalParticle> &part_in, bool GenerateAntiParticles = true);
    
    /**
     * \brief Writes the particle list to file.
     * 
     * Note: this does NOT write the decays.
     * The decays can be written with the WriteDecaysToFile() function.
     * 
     * \param OutputFile Path to the output file.
     * \param WriteAntiParticles Whether antiparticles are to be written to the file as well.
     */
    void WriteTableToFile(std::string OutputFile = "", bool WriteAntiParticles = false);

    /**
     * \brief Load the decay channels for all particles from a file.
     * 
     * \param DecaysFile Path to the file with decays.
     * \param GenerateAntiParticles Whether the decays of antiparticles are to be generated from particles automatically
     *        using the GetDecaysFromAntiParticle() method.
     */
    void LoadDecays(std::string DecaysFile = "", bool GenerateAntiParticles = true);
    
    /**
     * \brief Writes the decay channels to a file.
     * 
     * \param OutputFile Path to the output file.
     * \param WriteAntiParticles Whether the decays of antiparticles are to be written to the file as well.
     */
    void WriteDecaysToFile(std::string OutputFile = "", bool WriteAntiParticles = false);

    /**
     * \brief Normalize branching ratios for all particles
     *        such that they add up to 100%.
     * 
     */
    void NormalizeBranchingRatios();

    /**
     * \brief Restore the original values of all the branching ratios.
     * 
     */
    void RestoreBranchingRatios();

    //@{
    /**
     * \brief Sets the CalculationType() method to evaluate quantum statistics.
     * 
     * \param type Method to evaluate quantum statistics.
     */
    void SetCalculationType(IdealGasFunctions::QStatsCalculationType type);
    IdealGasFunctions::QStatsCalculationType QStatsCalculationType() const { return m_QStatsCalculationType; }
    //@}

    /**
     * \brief Set the number of terms in the cluster expansion method.
     * 
     * Sets the same value for all particles.
     * To set individually for each particle
     * use ThermalParticle::SetClusterExpansionOrder().
     * 
     * \param order Number of terms.
     */
    void SetClusterExpansionOrder(int order);

    //@{
    /**
     * \brief Set (or get) the ThermalParticle::ResonanceWidthShape for all particles.
     * 
     * This global setting can be overriden for individual particles
     * by ThermalParticle::SetResonanceWidthShape().
     * 
     * \param shape ThermalParticle::ResonanceWidthShape
     */
    void SetResonanceWidthShape(ThermalParticle::ResonanceWidthShape shape);
    ThermalParticle::ResonanceWidthShape ResonanceWidthShape() const { return m_ResonanceWidthShape; }
    //@}

    //@{
      /**
       * \brief Set (or get) the ThermalParticle::ResonanceWidthIntegration scheme for
       *        all particles.
       * 
       * \param type ThermalParticle::ResonanceWidthIntegration
       */
    void SetResonanceWidthIntegrationType(ThermalParticle::ResonanceWidthIntegration type);
    ThermalParticle::ResonanceWidthIntegration ResonanceWidthIntegrationType() const { return m_ResonanceWidthIntegrationType; }
    //@}

    /**
     * \brief Get the name of particle species
     *        with the specified PDG ID.
     * 
     * If the requested PDG ID is not in the particle list,
     * returns "???".
     * 
     * \param pdgid The requested PDG ID.
     * \return std::string Particle name.
     */
    std::string GetNameFromPDG(long long pdgid);

    /// Whether the particle list contains
    /// particles with a non-zero baryon charge.
    bool hasBaryons() const { return (m_NumBaryons > 0); }

    /// Whether the particle list contains
    /// particles with a non-zero electric charge.
    bool hasCharged() const { return (m_NumCharged > 0); }

    /// Whether the particle list contains
    /// particles with a non-zero strangeness.
    bool hasStrange() const { return (m_NumStrange > 0); }

    /// Whether the particle list contains
    /// particles with a non-zero charm.
    bool hasCharmed() const { return (m_NumCharmed > 0); }

    /// Number of different particle species in the list
    int ComponentsNumber() const { return static_cast<int>(m_Particles.size()); }

    /**
     * \brief Returns the vector of all particle species.
     * 
     * \return const std::vector<ThermalParticle>& The vector of all particle species.
     */
    const std::vector<ThermalParticle>& Particles() const { return m_Particles; }

    //@{
      /**
       * \brief ThermalParticle object corresponding
       *        to particle species with a provided 0-based index.
       * 
       * \param id 0-based index of a particle specie.
       * \return const ThermalParticle& ThermalParticle object reference.
       */
    const ThermalParticle& Particle(int id) const;
    ThermalParticle& Particle(int id);
    //@}

    /**
     * \brief ThermalParticle object corresponding
     *        to particle species with a provided PDG ID.
     * 
     * If the provided PDG ID does not exist in the list,
     * program quits.
     * 
     * \param pdgid PDG ID of a particle specie.
     * \return ThermalParticle& ThermalParticle object reference.
     */
    ThermalParticle& ParticleByPDG(long long pdgid);

    /**
     * \brief Transforms PDG ID to a 0-based particle id number.
     * 
     * Returns -1 if the provided PDG ID does not exist in the list.
     * 
     * \param pdgid PDG ID.
     * \return int  0-based particle id number.
     */
    int  PdgToId(long long pdgid)    /*const*/ { return (m_PDGtoID.count(pdgid) > 0) ? m_PDGtoID[pdgid] : -1; }
    
    /**
     * \brief Transforms 0-based particle id number to a PDG ID.
     * 
     * Returns 0 if the provided id number is out of bounds.
     * 
     * \param id   0-based particle id number.
     * \return int PDG ID.
     */
    long long IdToPdg(int id)      const { return (id >= 0 && id < static_cast<int>(m_Particles.size())) ? m_Particles[id].PdgId() : 0; }

    /**
     * \brief Adds a new particle specie to the list.
     * 
     * Note that this method does NOT generate an antiparticle.
     * 
     * \param part ThermalParticle object corresponding to a new particle specie added.
     */
    void AddParticle(const ThermalParticle & part);

    /**
     * \brief Removes particle specie with specified 0-based particle id number
     *        from the list.
     * 
     * \param ind 0-based particle id number of particle specie to be removed.
     */
    void RemoveParticleAt(int ind);

    /**
     * Checks whether cumulative charges (B, Q, S, C) of decay products match those of decaying particle with index ind
     */
    bool CheckDecayChargesConservation(int ind) const;

    bool operator==(const ThermalParticleSystem &rhs) const;
    bool operator!=(const ThermalParticleSystem &rhs) const { return !(*this == rhs); }

    /// Fills the PdgToId() map between PDG ID numbers
    /// and the 0-based indices of all particles
    void FillPdgMap();

    /// Mode list to sort particles species
    enum SortModeType {
      ByMass = 0,
      ByMassAndPDG = 1,
      ByBaryonAndMassAndPDG = 2
    };

    /// Sets the mode to sort particle species (see SortModeType)
    /// Use carefully! Need to recalculate all model calculations after this!
    void SetSortMode(SortModeType type) { m_SortMode = type; FinalizeList(); ProcessDecays(); }

    /// Current mode to sort particle species
    SortModeType SortMode() const { return m_SortMode; }

    /// Sorts the particles by their mass,
    /// fills the PdgToId() map between PDG ID numbers
    /// and the 0-based indices of all particles,
    /// and determines the decay types of each species
    void FinalizeList();

    /**
     * \brief Determines the decay type of a given particle specie.
     * 
     * Determines whether a given particle is stable,
     * or strongly, electromagnetically, or weakly decaying particle.
     * Method first determines whether the PDG ID of a given particle
     * is corresponds to one of the known stable, weakly or electromagnetically decaying particle.
     * If not, the particle is considered to be strongly decaying if its stability flag is set
     * to false. If the stability flag is true, then particle is considered to be weakly decaying
     * if it contains strange or charm quarks, or stable otherwise. 
     * 
     * \param part ThermalParticle object of a given particle specie.
     * \return ParticleDecay::DecayType The decay type of a given particle.
     */
    static ParticleDecayType::DecayType DecayTypeByParticleType(const ThermalParticle &part);

  private:
    void GoResonance(int ind, int startind, double BR);

    void GoResonanceByFeeddown(int ind, int startind, double BR, Feeddown::Type feeddown);

    std::vector<double> GoResonanceDecayProbs(int ind, int goalind, bool firstdecay = false);

    std::vector<double> GoResonanceDecayProbsCharge(int ind, int nch, bool firstdecay = false);

    ResonanceFinalStatesDistribution GoResonanceDecayDistributions(int ind, bool firstdecay = false);

    void LoadTable_OldFormat(std::ifstream &fin, bool GenerateAntiParticles = true, double mcut = 1.e9);

    void LoadTable_NewFormat(std::ifstream &fin, bool GenerateAntiParticles = true, double mcut = 1.e9);

    void ReadDecays_OldFormat(std::ifstream &fin);

    void ReadDecays_NewFormat(std::ifstream &fin);

  private:
    std::vector<ThermalParticle>    m_Particles;
    std::map<long long, int>              m_PDGtoID;
    int m_NumBaryons;
    int m_NumCharged;
    int m_NumStrange;
    int m_NumCharmed;

    int m_NumberOfParticles;

    ThermalParticle::ResonanceWidthIntegration m_ResonanceWidthIntegrationType;

    ThermalParticle::ResonanceWidthShape m_ResonanceWidthShape;

    IdealGasFunctions::QStatsCalculationType m_QStatsCalculationType;

    std::vector<DecayContributionsToAllParticles> m_DecayContributionsByFeeddown;

    DecayCumulantsContributionsToAllParticles m_DecayCumulants;

    DecayProbabilityDistributionsToAllParticles m_DecayProbabilities;

    std::vector<ResonanceFinalStatesDistribution> m_ResonanceFinalStatesDistributions;

    // Map for DP-based calculations of decay distributions
    std::vector<ResonanceFinalStatesDistribution> m_DecayDistributionsMap;

    SortModeType m_SortMode;
  };

  /// Contains several helper routines.
  namespace CuteHRGHelper {
    std::vector<std::string> split(const std::string &s, char delim);
    void cutDecayDistributionsVector(std::vector<std::pair<double, std::vector<int> > > &vect, int maxsize = 1000);
  }

} // namespace thermalfist

#endif
