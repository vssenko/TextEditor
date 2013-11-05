// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>
// Файлы заголовков C RunTime
#include <Commdlg.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <Windowsx.h>
// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
#include "ExtendedChar.h"
#include "BaseWindow.h"
#include "BaseController.h"
#include "Text.h"
#include "DrawingController.h"
#include "ActionController.h"
#include "ScalingController.h"
#include "Filemanager.h"
#include "HistoryController.h"
#include "AllWhatYouWantController.h"
#include "TextEditorWindow.h"
#include "TextEditor.h"