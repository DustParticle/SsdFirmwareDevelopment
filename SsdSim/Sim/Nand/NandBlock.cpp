#include <cstring>

#include "Nand/NandBlock.h"

NandBlock::NandBlock(U32 pagesPerBlock, U32 totalBytesPerPage)
{
	_PagesPerBlock = pagesPerBlock;
	_TotalBytesPerPage = totalBytesPerPage;

	_ErasedBuffer = std::unique_ptr<U8[]>(new U8[_TotalBytesPerPage]);
	for (U32 i = 0; i < _TotalBytesPerPage; ++i)
	{
		_ErasedBuffer[i] = ERASED_PATTERN;
	}
}

NandBlock::NandBlock(NandBlock&& rhs)
{
	_Buffer = std::move(rhs._Buffer);
	_ErasedBuffer = std::move(rhs._ErasedBuffer);

	_PagesPerBlock = rhs._PagesPerBlock;
	_TotalBytesPerPage = rhs._TotalBytesPerPage;
}

void NandBlock::Erase()
{
	_Buffer.reset();
}

void NandBlock::WritePage(tPageInBlock page, const U8* const pInData)
{
	if (nullptr == _Buffer)
	{
		_Buffer = std::unique_ptr<U8[]>(new U8[_PagesPerBlock * _TotalBytesPerPage]);
	}

	std::memcpy((void*)&_Buffer[page._ * _TotalBytesPerPage], pInData, _TotalBytesPerPage);
}

void NandBlock::ReadPage(tPageInBlock page, U8* const pOutData)
{
	auto pData = (nullptr == _Buffer) ? &_ErasedBuffer[0] : &_Buffer[page._ * _TotalBytesPerPage];

	std::memcpy((void*)pOutData, (void*)pData, _TotalBytesPerPage);
}

