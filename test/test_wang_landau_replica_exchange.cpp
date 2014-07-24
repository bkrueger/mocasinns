#include "test_wang_landau_replica_exchange.hpp"

CppUnit::Test* TestWangLandauReplicaExchange::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestWangLandauReplicaExchange");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestWangLandauReplicaExchange>("TestWangLandauReplicaExchange: test_do_wang_landau_replica_exchange_simulation", &TestWangLandauReplicaExchange::test_do_wang_landau_replica_exchange_simulation) );

  return suite_of_tests;
}

void TestWangLandauReplicaExchange::setUp()
{
  // 1d-lattice of Ising spins
  std::vector<unsigned int> size_1d;
  size_1d.push_back(16);
  parameters_1d.modification_factor_initial = 1.0;
  parameters_1d.modification_factor_final = 1e-6;
  parameters_1d.modification_factor_multiplier = 0.9;
  parameters_1d.flatness = 0.8;
  parameters_1d.sweep_steps = 1000;
  parameters_1d.simulations_per_replica = 2;
  parameters_1d.sweeps_per_replica_exchange = 10;
  std::vector<std::pair<int, int> > energy_ranges;
  energy_ranges.push_back(std::pair<int, int>(-16,8));
  energy_ranges.push_back(std::pair<int, int>(-8,16));
  parameters_1d.energy_ranges = energy_ranges;

  test_configuration_spaces_1d.push_back(new IsingConfiguration1d(size_1d));
  test_configuration_spaces_1d.push_back(new IsingConfiguration1d(size_1d));
  test_configuration_spaces_1d.push_back(new IsingConfiguration1d(size_1d));
  test_configuration_spaces_1d.push_back(new IsingConfiguration1d(size_1d));
  index_type_1d index_0; index_0[0] = 0; index_type_1d index_1; index_1[0] = 1;
  index_type_1d index_2; index_2[0] = 2; index_type_1d index_3; index_3[0] = 3;
  index_type_1d index_4; index_4[0] = 4; index_type_1d index_5; index_5[0] = 5;
  index_type_1d index_6; index_6[0] = 6; index_type_1d index_7; index_7[0] = 7;
  index_type_1d index_8; index_8[0] = 8; index_type_1d index_9; index_9[0] = 9;
  index_type_1d index_10; index_10[0] = 10; index_type_1d index_11; index_11[0] = 11;
  index_type_1d index_12; index_12[0] = 12; index_type_1d index_13; index_13[0] = 13;
  index_type_1d index_14; index_14[0] = 14; index_type_1d index_15; index_15[0] = 15;
  test_configuration_spaces_1d[2]->set_spin(index_0, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_0, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_2, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_4, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_4, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_6, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_6, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_8, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_8, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_10, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_10, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_12, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_12, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_14, Gespinst::IsingSpin(-1)); test_configuration_spaces_1d[3]->set_spin(index_14, Gespinst::IsingSpin(-1));
  test_configuration_spaces_1d[2]->set_spin(index_1, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_1, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_3, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_3, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_5, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_5, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_7, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_7, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_9, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_9, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_11, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_11, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_13, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_13, Gespinst::IsingSpin(1));
  test_configuration_spaces_1d[2]->set_spin(index_15, Gespinst::IsingSpin(1)); test_configuration_spaces_1d[3]->set_spin(index_15, Gespinst::IsingSpin(1));
  test_ising_simulation_1d = new IsingSimulation1d(parameters_1d, test_configuration_spaces_1d.begin(), test_configuration_spaces_1d.end());
}

void TestWangLandauReplicaExchange::tearDown()
{
  delete test_ising_simulation_1d;
  delete test_configuration_spaces_1d[0];
  delete test_configuration_spaces_1d[1];
  delete test_configuration_spaces_1d[2];
  delete test_configuration_spaces_1d[3];
}

void TestWangLandauReplicaExchange::test_do_wang_landau_replica_exchange_simulation()
{
  // Register the information handler
  //  test_ising_simulation_1d->signal_handler_modfac_change.connect(IsingSimulation1d::SimulationStatus());

  // Do a complete Wang-Landau simulation in the 1d case
  test_ising_simulation_1d->do_wang_landau_replica_exchange_simulation();

  // Test the density of states histocrete
  Histograms::Histocrete<int, double> entropy_estimation_1d_1 = test_ising_simulation_1d->get_log_density_of_states()[0];
  Histograms::Histocrete<int, double> entropy_estimation_1d_2 = test_ising_simulation_1d->get_log_density_of_states()[1];
  // std::cout << "DOS_1[-16] = " << entropy_estimation_1d_1[-16] << std::endl;
  // std::cout << "DOS_1[-12] = " << entropy_estimation_1d_1[-12] << std::endl;
  // std::cout << "DOS_1[-8] = " << entropy_estimation_1d_1[-8] << std::endl;
  // std::cout << "DOS_1[-4] = " << entropy_estimation_1d_1[-4] << std::endl;
  // std::cout << "DOS_1[0] = " << entropy_estimation_1d_1[0] << std::endl;
  // std::cout << "DOS_1[4] = " << entropy_estimation_1d_1[4] << std::endl;
  // std::cout << "DOS_1[8] = " << entropy_estimation_1d_1[8] << std::endl;
  // std::cout << "DOS_2[-8] = " << entropy_estimation_1d_2[-8] << std::endl;
  // std::cout << "DOS_2[-4] = " << entropy_estimation_1d_2[-4] << std::endl;
  // std::cout << "DOS_2[0] = " << entropy_estimation_1d_2[0] << std::endl;
  // std::cout << "DOS_2[4] = " << entropy_estimation_1d_2[4] << std::endl;
  // std::cout << "DOS_2[8] = " << entropy_estimation_1d_2[8] << std::endl;
  // std::cout << "DOS_2[12] = " << entropy_estimation_1d_2[12] << std::endl;
  // std::cout << "DOS_2[16] = " << entropy_estimation_1d_2[16] << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, entropy_estimation_1d_1[-16], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.7875, entropy_estimation_1d_1[-12], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066, entropy_estimation_1d_1[-8], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882, entropy_estimation_1d_1[-4], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4627, entropy_estimation_1d_1[0], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882, entropy_estimation_1d_1[4], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066, entropy_estimation_1d_1[8], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066 - 7.5066, entropy_estimation_1d_2[-8], 0.125);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882 - 7.5066, entropy_estimation_1d_2[-4], 0.125);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4627 - 7.5066, entropy_estimation_1d_2[0], 0.125);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882 - 7.5066, entropy_estimation_1d_2[4], 0.125);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066 - 7.5066, entropy_estimation_1d_2[8], 0.125);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.7875 - 7.5066, entropy_estimation_1d_2[12], 0.125);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0 - 7.5066, entropy_estimation_1d_2[16], 0.125);
}
