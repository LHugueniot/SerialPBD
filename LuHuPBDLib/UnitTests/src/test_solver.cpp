#include "gtest/gtest.h"
#include <PBDLib>

TEST(solver, constructor)
{
    LuHu::solver TestSolver(3.0, glm::vec3(0,-0.98,0));
    ASSERT_EQ(TestSolver.getGrav(), glm::vec3(0,-0.98,0));
}
