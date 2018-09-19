#include "pch.h"

#include "Nand/NandDevice.h"
#include "Nand/NandHal.h"

TEST(NandDeviceTest, Basic) {
	constexpr U32 blockCount = 2;
	constexpr U32 pagesPerBlock = 256;
	constexpr U32 bytesPerPage = 8192;

	NandDevice nandDevice(blockCount, pagesPerBlock, bytesPerPage);

	U8 pDataToWrite[bytesPerPage];
	for (auto i(0); i < bytesPerPage; ++i)
	{
		pDataToWrite[i] = i % 255;
	}
	U8 pDataRead[bytesPerPage];

	tBlockInChip block;
	block._ = 0;
	for ( ; block._ < blockCount; ++block._)
	{
		tPageInBlock page;
		page._ = 0;
		for (; page._ < pagesPerBlock; ++page._)
		{
			nandDevice.WritePage(block, page, pDataToWrite);

			nandDevice.ReadPage(block, page, pDataRead);
			for (auto i(0); i < bytesPerPage; ++i)
			{
				ASSERT_EQ(pDataToWrite[i], pDataRead[i]);
			}
		}

		nandDevice.EraseBlock(block);
		for (; page._ < pagesPerBlock; ++page._)
		{
			nandDevice.ReadPage(block, page, pDataRead);
			for (auto i = 0; i < bytesPerPage; ++i)
			{
				ASSERT_EQ(NandBlock::ERASED_PATTERN, pDataRead[i]);
			}
		}
	}
}

TEST(NandHalTest, Basic) {
	constexpr U8 chips = 2;
	constexpr U32 blocks = 4;
	constexpr U32 pages = 128;
	constexpr U32 bytes = 8192;

	NandHal nandHal;
	nandHal.PreInit(chips, blocks, pages, bytes);
	nandHal.Init();

	U8 pDataToWrite[bytes];
	for (auto i(0); i < bytes; ++i)
	{
		pDataToWrite[i] = i % 255;
	}
	U8 pDataRead[bytes];

	tChip chip;
	chip._ = 0;
	for ( ; chip._ < chips; ++chip._)
	{
		tBlockInChip block;
		block._ = 0;
		for (; block._ < blocks; ++block._)
		{
			tPageInBlock page;
			page._ = 0;
			for (; page._ < pages; ++page._)
			{
				nandHal.WritePage(chip, block, page, pDataToWrite);

				nandHal.ReadPage(chip, block, page, pDataRead);
				for (auto i(0); i < bytes; ++i)
				{
					ASSERT_EQ(pDataToWrite[i], pDataRead[i]);
				}
			}

			nandHal.EraseBlock(chip, block);
			for (; page._ < pages; ++page._)
			{
				nandHal.ReadPage(chip, block, page, pDataRead);
				for (auto i = 0; i < bytes; ++i)
				{
					ASSERT_EQ(NandBlock::ERASED_PATTERN, pDataRead[i]);
				}
			}
		}
	}
}