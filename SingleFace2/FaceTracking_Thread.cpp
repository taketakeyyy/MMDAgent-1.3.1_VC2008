/* ----------------------------------------------------------------- */
/*           The Toolkit for Building Voice Interaction Systems      */
/*           "MMDAgent" developed by MMDAgent Project Team           */
/*           http://www.mmdagent.jp/                                 */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*  All rights reserved.                                             */
/*                                                                   */
/*  Copyright (c) 2012       CUBE370                                 */
/*  All rights reserved.                                             */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAgent project team nor the names of  */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/***** include *****/
#include <iostream>
#include "MMDAgent.h"
#include "FaceTracking_Thread.h"


/***** mainThread: メインスレッド *****/
static void mainThread(void *param)
{
	FaceTracking_Thread *FaceTracking_thread = (FaceTracking_Thread *) param;
	FaceTracking_thread->run();
}

/***** Skeleton_Thread::initialize: スレッド初期化 *****/
void FaceTracking_Thread::initialize()
{
	m_mmdagent = NULL;

	m_mutex = NULL;
	m_thread = -1;

	m_kill = false;
}

/***** Skeleton_Thread::clear: スレッド開放 *****/
void FaceTracking_Thread::clear()
{
	m_kill = true;

	if ((m_mutex != NULL) || (m_thread >= 0))
	{
		if (m_thread >= 0)
		{
			glfwWaitThread(m_thread, GLFW_WAIT);
			glfwDestroyThread(m_thread);
		}
		if (m_mutex != NULL)
		{
			glfwDestroyMutex(m_mutex);
		}
		glfwTerminate();
	}

	initialize();
}

/***** Skeleton_Thread::Skeleton_Thread: コンストラクタ *****/
FaceTracking_Thread::FaceTracking_Thread()
{
   initialize();
}

/***** Skeleton_Thread::~Skeleton_Thread: デストラクタ *****/
FaceTracking_Thread::~FaceTracking_Thread()
{
	clear();
}

/***** Skeleton_Thread::setupAndStart: スレッド初期化 & スタート *****/
void FaceTracking_Thread::setupAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;

	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);
	if (m_mutex == NULL || m_thread < 0)
	{
		clear();
		return;
	}
}

/***** Skeleton_Thread::stopAndRelease: スレッド停止 & 開放 *****/
void FaceTracking_Thread::stopAndRelease()
{
	clear();
}

/***** Skeleton_Thread::run: メインループ *****/
void FaceTracking_Thread::run()
{
	while (m_kill == false)
	{
		glfwLockMutex(m_mutex);
		//繰り返すことはここでやるぽい？


		glfwUnlockMutex(m_mutex);
	}
}

/***** Skeleton_Thread::isRunning: 実行中のチェック *****/
bool FaceTracking_Thread::isRunning()
{
	if ((m_kill == true) || (m_mutex == NULL) || (m_thread < 0))
	{
		return false;
	}
	else
	{
		return true;
	}
}