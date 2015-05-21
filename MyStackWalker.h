#pragma once

class MyStackWalker : public StackWalker
{
public:
  MyStackWalker();
  MyStackWalker(DWORD dwProcessId, HANDLE hProcess);
  virtual void OnOutput(LPCSTR szText);
};
