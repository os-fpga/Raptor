/*
Copyright 2022 The Foedag team

GPL License

Copyright (c) 2022 The Open-Source FPGA Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <filesystem>

#include "Configuration/CFGCompiler/CFGCompiler.h"
#include "Main/Foedag.h"
#include "ProgrammerGui/ProgrammerMain.h"
#include "Utils/FileUtils.h"

QWidget* mainWindowBuilder(FOEDAG::Session* session) {
  auto m = new FOEDAG::ProgrammerMain{};
  m->setWindowTitle("Rapid Programmer");

  std::filesystem::path exeDirPath =
      QCoreApplication::applicationDirPath().toStdString();
  std::filesystem::path installDir = exeDirPath.parent_path();
  std::filesystem::path dataDir =
      installDir / "share" / "raptor" / "configuration";
  auto files = FOEDAG::FileUtils::FindFilesByExtension(dataDir, ".cfg");
  if (!files.empty())
    m->setCfgFile(QString::fromStdString(files.front().string()));

  return m;
}

void registerAllCommands(QWidget* widget, FOEDAG::Session* session) {
  auto compiler = session->GetCompiler();
  auto cfgCompiler = new FOEDAG::CFGCompiler(compiler);
  compiler->SetConfiguration(cfgCompiler);
  cfgCompiler->RegisterCommands(session->TclInterp(), false);
}

int main(int argc, char* argv[]) {
  FOEDAG::CommandLine* cmd = new FOEDAG::CommandLine(argc, argv);
  cmd->processArgs();

  FOEDAG::GUI_TYPE guiType =
      FOEDAG::Foedag::getGuiType(cmd->WithQt(), cmd->WithQml());

  FOEDAG::Compiler* compiler = new FOEDAG::Compiler;
  FOEDAG::Settings* settings = new FOEDAG::Settings();
  FOEDAG::Foedag* foedag = new FOEDAG::Foedag(
      cmd, mainWindowBuilder, registerAllCommands, compiler, settings);
  return foedag->init(guiType);
}
