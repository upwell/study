#include "gtest/gtest.h"

#include "filemapping.h"
#include "searchresult.h"

class FooTest : public ::testing::Test {
protected:
    static void SetUpTestCase() {
        shared_resource_ = new FileMapping();
        shared_resource_->Init();
    }

    static void TearDownTestCase() {
        delete shared_resource_;
        shared_resource_ = NULL;
    }

    static FileMapping* shared_resource_;
};

FileMapping* FooTest::shared_resource_ = NULL;

TEST_F(FooTest, Add)
{
    FooTest::shared_resource_->AddFile("//nj/home", "//nas1/home");
    {
        SearchResult result;

        FooTest::shared_resource_->GetRPath("//nj/home", result);
        EXPECT_TRUE(result.IsFound());
        EXPECT_TRUE(result.IsExactFound());
        EXPECT_FALSE(result.IsParentFound());

        EXPECT_STREQ("//nas1/home", result.GetExactRPath().c_str());
    }

    {
        SearchResult result;
        FooTest::shared_resource_->GetRPath("//nj/home/iwss/notfound", result);
        EXPECT_TRUE(result.IsFound());
        EXPECT_FALSE(result.IsExactFound());
        EXPECT_TRUE(result.IsParentFound());
        EXPECT_STREQ("//nas1/home/iwss/notfound", result.GetExactRPath().c_str());
    }

    {
        SearchResult result;
        FooTest::shared_resource_->GetRPath("//nj/ooo", result);
        EXPECT_FALSE(result.IsFound());
    }

    {
        SearchResult result;
        FooTest::shared_resource_->GetRPath("//nba/he", result);
        EXPECT_FALSE(result.IsFound());
    }

}
