# ----------------------------------------------------------------- #
#           MMDAgent "Sample Script"                                #
#           released by MMDAgent Project Team                       #
#           http://www.mmdagent.jp/                                 #
# ----------------------------------------------------------------- #
#                                                                   #
#  Copyright (c) 2009-2012  Nagoya Institute of Technology          #
#                           Department of Computer Science          #
#                                                                   #
# Some rights reserved.                                             #
#                                                                   #
# This work is licensed under the Creative Commons Attribution 3.0  #
# license.                                                          #
#                                                                   #
# You are free:                                                     #
#  * to Share - to copy, distribute and transmit the work           #
#  * to Remix - to adapt the work                                   #
# Under the following conditions:                                   #
#  * Attribution - You must attribute the work in the manner        #
#    specified by the author or licensor (but not in any way that   #
#    suggests that they endorse you or your use of the work).       #
# With the understanding that:                                      #
#  * Waiver - Any of the above conditions can be waived if you get  #
#    permission from the copyright holder.                          #
#  * Public Domain - Where the work or any of its elements is in    #
#    the public domain under applicable law, that status is in no   #
#    way affected by the license.                                   #
#  * Other Rights - In no way are any of the following rights       #
#    affected by the license:                                       #
#     - Your fair dealing or fair use rights, or other applicable   #
#       copyright exceptions and limitations;                       #
#     - The author's moral rights;                                  #
#     - Rights other persons may have either in the work itself or  #
#       in how the work is used, such as publicity or privacy       #
#       rights.                                                     #
#  * Notice - For any reuse or distribution, you must make clear to #
#    others the license terms of this work. The best way to do this #
#    is with a link to this web page.                               #
#                                                                   #
# See http://creativecommons.org/ for details.                      #
# ----------------------------------------------------------------- #

# 1st field: state before transition
# 2nd field: state after transition
# 3rd field: event (input message)
# 4th field: command (output message)
#
# Model
# MODEL_ADD|(model alias)|(model file name)|(x position),(y position),(z position)|(x rotation),(y rotation),(z rotation)|(ON or OFF for cartoon)|(parent model alias)|(parent bone name)
# MODEL_CHANGE|(model alias)|(model file name)
# MODEL_DELETE|(model alias)
# MODEL_EVENT_ADD|(model alias)
# MODEL_EVENT_CHANGE|(model alias)
# MODEL_EVENT_DELETE|(model alias)
#
# Motion
# MOTION_ADD|(model alias)|(motion alias)|(motion file name)|(FULL or PART)|(ONCE or LOOP)|(ON or OFF for smooth)|(ON or OFF for repos)
# MOTION_ACCELERATE|(model alias)|(motion alias)|(speed)|(duration)|(specified time for end)
# MOTION_CHANGE|(model alias)|(motion alias)|(motion file name)
# MOTION_DELETE|(mpdel alias)|(model alias)
# MOTION_EVENT_ADD|(model alias)|(motion alias)
# MOTION_EVENT_ACCELERATE|(model alias)|(motion alias)
# MOTION_EVENT_CHANGE|(model alias)|(model alias)
# MOTION_EVENT_DELETE|(model alias)|(motion alias)
#
# Move and Rotate
# MOVE_START|(model alias)|(x position),(y position),(z position)|(GLOBAL or LOCAL position)|(move speed)
# MOVE_STOP|(model alias)
# MOVE_EVENT_START|(model alias)
# MOVE_EVENT_STOP|(model alias)
# TURN_START|(model alias)|(x position),(y position),(z position)|(GLOBAL or LOCAL position)|(rotation speed)
# TURN_STOP|(model alias)
# TURN_EVENT_START|(model alias)
# TURN_EVENT_STOP|(model alias)
# ROTATE_START|(model alias)|(x rotation),(y rotataion),(z rotation)|(GLOBAL or LOCAL rotation)|(rotation speed)
# ROTATE_STOP|(model alias)
# ROTATE_EVENT_START|(model alias)
# ROTATE_EVENT_STOP|(model alias)
#
# Sound
# SOUND_START|(sound alias)|(sound file name)
# SOUND_STOP|(sound alias)
# SOUND_EVENT_START|(sound alias)
# SOUND_EVENT_STOP|(sound alias)
#
# Stage
# STAGE|(stage file name)
# STAGE|(bitmap file name for floor),(bitmap file name for background)
#
# light
# LIGHTCOLOR|(Red),(Green),(Blue)
# LIGHTDIRECTION|(x position),(y position),(z position)
# 
# Camera
# CAMERA|(x position),(y position),(z position)|(x rotation),(y rotation),(z rotation)|(distance)|(fovy)|(time)
# CAMERA|(motion file name)
#
# Speech recognition
# RECOG_EVENT_START
# RECOG_EVENT_STOP|(word sequence)
#
# Speech synthesis
# SYNTH_START|(model alias)|(voice alias)|(synthesized text)
# SYNTH_STOP|(model alias)
# SYNTH_EVENT_START|(model alias)
# SYNTH_EVENT_STOP|(model alias)
# LIPSYNC_START|(model alias)|(phoneme and milli second pair sequence)
# LIPSYNC_STOP|(model alias)
# LIPSYNC_EVENT_START|(model alias)
# LIPSYNC_EVENT_STOP|(model alias)
#
# Variable
# VALUE_SET|(variable alias)|(value)
# VALUE_SET|(variable alias)|(minimum value for random)|(maximum value for random)
# VALUE_UNSET|(variable alias)
# VALUE_EVAL|(variable alias)|(EQ or NE or LE or LT or GE or GT for evaluation)|(value)
# VALUE_EVENT_SET|(variable alias)
# VALUE_EVENT_UNSET|(variable alias)
# VALUE_EVENT_EVAL|(variable alias)|(EQ or NE or LE or LT or GE or GT for evaluation)|(value)|(TRUE or FALSE)
# TIMER_START|(count down alias)|(value)
# TIMER_STOP|(count down alias)
# TIMER_EVENT_START|(count down alias)
# TIMER_EVENT_STOP|(count down alias)
#
# Plugin
# PLUGIN_ENABLE|(plugin name)
# PLUGIN_DISABLE|(plugin name)
# PLUGIN_EVENT_ENABLE|(plugin name)
# PLUGIN_EVENT_DISABLE|(plugin name)
#
# Other events
# DRAGANDDROP|(file name)
# KEY|(key name)
#
# Other commands
# EXECUTE|(file name)
# KEY_POST|(window class name)|(key name)|(ON or OFF for shift-key)|(ON or OFF for ctrl-key)|(On or OFF for alt-key)

# 0011-0020 Initialization

0    11   <eps>                               MODEL_ADD|bootscreen|Accessory\bootscreen\bootscreen.pmd|0.0,12.85,17.6|0.0,0.0,0.0|OFF
11   11   MODEL_EVENT_ADD|bootscreen          MODEL_ADD|mei|Model\mei\mei.pmd|-6.0,0.0,-14.0
11   12   MODEL_EVENT_ADD|mei                 MODEL_ADD|mei2|Model\mei_black_jacket\mei_black_jacket.pmd|6.0,0.0,-14.0
12   13   <eps>                               MODEL_ADD|menu|Accessory\menu\menu.pmd|0.0,-4.5,0.0|0.0,0.0,0.0|ON|mei
13   14   <eps>                               MOTION_ADD|menu|rotate|Motion\menu_rotation\menu_rotation.vmd|FULL|LOOP|OFF|OFF
14   15   <eps>                               STAGE|Stage\building2\floor.bmp,Stage\building2\background.bmp
15   1016 <eps>                               MOTION_ADD|mei|base|Motion\mei_wait\mei_wait.vmd|FULL|LOOP|ON|OFF
1016 16   <eps>                               MOTION_ADD|mei2|base|Motion\mei_wait\mei_wait.vmd|FULL|LOOP|ON|OFF

###
#mei��������̒ǉ�����B
#16   1011 <eps>                               MODEL_ADD|mei2|Model\mei_black_jacket\mei_black_jacket.pmd|2.0,0.0,-14.0
#1011 1012 <eps>                               MOTION_ADD|mei2|base|Motion\mei_wait\mei_wait.vmd|FULL|LOOP|ON|OFF
#
###

16   17   <eps>                               TIMER_START|bootscreen|1.5
17   2    TIMER_EVENT_STOP|bootscreen         MODEL_DELETE|bootscreen

# 0021-0030 Idle behavior

2    21   <eps>                               TIMER_START|idle1|20
21   22   TIMER_EVENT_START|idle1             TIMER_START|idle2|40
22   23   TIMER_EVENT_START|idle2             TIMER_START|idle3|60
23   1    TIMER_EVENT_START|idle3             VALUE_SET|random|0|100
1    1    RECOG_EVENT_START                   MOTION_ADD|mei|listen|Expression\mei_listen\mei_listen.vmd|PART|ONCE
1    1    TIMER_EVENT_STOP|idle1              MOTION_ADD|mei|idle|Motion\mei_idle\mei_idle_boredom.vmd|PART|ONCE
1    1    TIMER_EVENT_STOP|idle2              MOTION_ADD|mei|idle|Motion\mei_idle\mei_idle_touch_clothes.vmd|PART|ONCE
1    2    TIMER_EVENT_STOP|idle3              MOTION_ADD|mei|idle|Motion\mei_idle\mei_idle_think.vmd|PART|ONCE

# 0031-0040 Hello

1    31   RECOG_EVENT_STOP|����ɂ���         SYNTH_START|mei|mei_voice_normal|����ɂ��́B
1    31   RECOG_EVENT_STOP|����ɂ���         SYNTH_START|mei|mei_voice_normal|����ɂ��́B
31   32   <eps>                               MOTION_ADD|mei|action|Motion\mei_greeting\mei_greeting.vmd|PART|ONCE
32   2    SYNTH_EVENT_STOP|mei                <eps>

# 0041-0050 Self introduction

1    41   RECOG_EVENT_STOP|���ȏЉ�           SYNTH_START|mei|mei_voice_normal|���C�ƌ����܂��B
1    41   RECOG_EVENT_STOP|���Ȃ�,�N          SYNTH_START|mei|mei_voice_normal|���C�ƌ����܂��B
1    41   RECOG_EVENT_STOP|�N,�N              SYNTH_START|mei|mei_voice_normal|���C�ƌ����܂��B
41   42   <eps>                               MOTION_ADD|mei|action|Motion\mei_self_introduction\mei_self_introduction.vmd|PART|ONCE
42   43   SYNTH_EVENT_STOP|mei                SYNTH_START|mei|mei_voice_normal|��낵�����肢���܂��B
43   2    SYNTH_EVENT_STOP|mei                <eps>

# 0051-0060 Thank you

1    51   RECOG_EVENT_STOP|���肪��           SYNTH_START|mei|mei_voice_normal|�ǂ��������܂��āB
1    51   RECOG_EVENT_STOP|���肪�Ƃ�         SYNTH_START|mei|mei_voice_normal|�ǂ��������܂��āB
1    51   RECOG_EVENT_STOP|�L�             SYNTH_START|mei|mei_voice_normal|�ǂ��������܂��āB
1    51   RECOG_EVENT_STOP|�L��           SYNTH_START|mei|mei_voice_normal|�ǂ��������܂��āB
51   52   <eps>                               MOTION_ADD|mei|expression|Expression\mei_happiness\mei_happiness.vmd|PART|ONCE
52   53   SYNTH_EVENT_STOP|mei                SYNTH_START|mei|mei_voice_happy|���ł��A�b�������Ă��������ˁB
53   54   <eps>                               MOTION_CHANGE|mei|base|Motion\mei_guide\mei_guide_happy.vmd
54   2    SYNTH_EVENT_STOP|mei                MOTION_CHANGE|mei|base|Motion\mei_wait\mei_wait.vmd

# 0061-0070 Positive comments

1    61   RECOG_EVENT_STOP|����             VALUE_EVAL|random|LE|80
1    61   RECOG_EVENT_STOP|���킢��           VALUE_EVAL|random|LE|80
1    61   RECOG_EVENT_STOP|�Y��               VALUE_EVAL|random|LE|80
1    61   RECOG_EVENT_STOP|���ꂢ             VALUE_EVAL|random|LE|80
61   62   VALUE_EVENT_EVAL|random|LE|80|TRUE  SYNTH_START|mei|mei_voice_bashful|�p���������ł��B
61   62   VALUE_EVENT_EVAL|random|LE|80|FALSE SYNTH_START|mei|mei_voice_bashful|���肪�Ƃ��B
62   63   <eps>                               MOTION_ADD|mei|expression|Expression\mei_bashfulness\mei_bashfulness.vmd|PART|ONCE
63   2    SYNTH_EVENT_STOP|mei                <eps>

# 0071-0090 Guide

1    71   RECOG_EVENT_STOP|�}����             MODEL_DELETE|menu
71   72   <eps>                               MODEL_ADD|panel|Accessory\map\map_library.pmd|0.0,2.8,2.5|0.0,0.0,0.0|ON|mei
72   73   <eps>                               MOTION_ADD|mei|action|Motion\mei_panel\mei_panel_on.vmd|PART|ONCE
73   74   <eps>                               MOTION_CHANGE|mei|base|Motion\mei_guide\mei_guide_normal.vmd
74   75   <eps>                               SYNTH_START|mei|mei_voice_normal|�}���ق́A���ʂ��猩��ƁA�E�O�̕����ɂ���܂��B
75   76   SYNTH_EVENT_STOP|mei                MOTION_ADD|mei|look|Motion\mei_look\mei_look_down.vmd|PART|ONCE
76   77   <eps>                               SYNTH_START|mei|mei_voice_normal|�L�����p�X�}�b�v�ł́A������ɂȂ�܂��B
77   78   <eps>                               MOTION_ADD|mei|action|Motion\mei_point\mei_point_center_buttom.vmd|PART|ONCE
78   79   SYNTH_EVENT_STOP|mei                MOTION_CHANGE|mei|base|Motion\mei_guide\mei_guide_happy.vmd
79   80   <eps>                               SYNTH_START|mei|mei_voice_normal|������ɂȂ�܂����H
80   81   SYNTH_EVENT_STOP|mei                MODEL_DELETE|panel
81   82   <eps>                               MODEL_ADD|menu|Accessory\menu\menu.pmd|0.0,-4.5,0.0|0.0,0.0,0.0|mei
82   83   <eps>                               MOTION_CHANGE|mei|base|Motion\mei_wait\mei_wait.vmd
83   2    <eps>                               MOTION_ADD|menu|rotate|Motion\menu_rotation\menu_rotation.vmd|FULL|LOOP|OFF

# 0091-0100 Bye

1    91   RECOG_EVENT_STOP|�o�C�o�C           SYNTH_START|mei|mei_voice_normal|���悤�Ȃ�B
1    91   RECOG_EVENT_STOP|���悤�Ȃ�         SYNTH_START|mei|mei_voice_normal|���悤�Ȃ�B
1    91   RECOG_EVENT_STOP|����Ȃ�           SYNTH_START|mei|mei_voice_normal|���悤�Ȃ�B
91   92   <eps>                               MOTION_ADD|mei|action|Motion\mei_bye\mei_bye.vmd|PART|ONCE
92   2    SYNTH_EVENT_STOP|mei                <eps>

# 0101-0110 Browsing

1    101  RECOG_EVENT_STOP|�z�[���y�[�W       EXECUTE|http://www.mmdagent.jp/
1    101  RECOG_EVENT_STOP|�l�l�c�`��������   EXECUTE|http://www.mmdagent.jp/
101  102  <eps>                               SYNTH_START|mei|mei_voice_normal|�l�l�c�`���������́A�z�[���y�[�W��\�����܂��B
102  2    SYNTH_EVENT_STOP|mei                <eps>

# 0111-0120 Screen

1    111  RECOG_EVENT_STOP|�t���X�N���[��     KEY_POST|MMDAgent|F|OFF
111  112  <eps>                               SYNTH_START|mei|mei_voice_normal|�X�N���[���̐ݒ���A�ύX���܂����B
112  2    SYNTH_EVENT_STOP|mei                <eps>

# �摜�\���T���v��
#1   141   KEY|1 SYNTH_START|mei|mei_voice_normal|����Ȃӂ��ɂ����Ђ������{���Ɏg���Ƃ��킢���ł���B
#141  142  <eps>         IMAGE_ADD|image|082.jpg|10.0,10.0|-10.0,15.0,-10.0|0.0,0.0,0.0|ON
#142  143   <eps>        MOTION_ADD|mei|action|Motion\mei_point\mei_point_right_center.vmd|PART|ONCE
3143  2    VOICEROID_EVENT_STOP|mei_voice_normal                <eps>
##�Ȃ񂩋�������������
# �摜�폜�T���v��
#1   2   KEY|2   IMAGE_DELETE|image


#���[�V�����e�X�g
#1 1 KEY|1                                  MOTION_ADD|mei|action1|Motion\mei_breath\mei_breath.vmd|PART|ONCE
#1 1 KEY|2                                  MOTION_ADD|mei|action2|Motion\mei_nod\nod2.vmd|FULL|ONCE
#1 1 KEY|3                                  MOTION_ADD|mei|action3|Motion\mei_surprise\mei_surprise_normal.vmd|FULL|ONCE
#1 1 KEY|4                                  MOTION_ADD|mei|action4|Motion\mei_bye\mei_bye.vmd|FULL|ONCE
#1 1 KEY|5                                  MOTION_ADD|mei|action5|Motion\mei_guide\mei_guide_happy.vmd|FULL|ONCE
#1 1 KEY|6                                  MOTION_ADD|mei|action6|Motion\mei_guide\mei_guide_sad.vmd|FULL|ONCE
#1 1 KEY|7                                  MOTION_ADD|mei|action7|Motion\mei_guide\mei_guide_normal.vmd|FULL|ONCE
#1 1 KEY|8                                  MOTION_ADD|mei|action8|Motion\mei_imagine\mei_imagine_left_normal.vmd|FULL|ONCE
#1 1 KEY|9                                  MOTION_ADD|mei|action9|Motion\mei_imagine\mei_imagine_left_small.vmd|FULL|ONCE
#1 1 KEY|0                                  MOTION_ADD|mei|action0|Motion\mei_imagine\mei_imagine_right_normal.vmd|FULL|ONCE

#1 1 KEY|0                                  MOTION_ADD|mei|action0|Motion\mei_panel\mei_panel_on.vmd|FULL|ONCE
#1 1 KEY|1                                  MOTION_ADD|mei|action1|Motion\mei_panel\mei_panel_off.vmd|FULL|ONCE
#1 1 KEY|2                                  MOTION_ADD|mei|action2|Motion\mei_call\mei_call.vmd|FULL|ONCE                #�u���[���v�݂����Ȋ���
#1 1 KEY|3                                  MOTION_ADD|mei|action3|Motion\mei_guts\mei_guts.vmd|FULL|ONCE                #�K�b�c�|�[�Y
#1 1 KEY|4                                  MOTION_ADD|mei|action4|Motion\mei_laugh\mei_laugh.vmd|FULL|ONCE              #�u���������v�݂����Ȋ���
#1 1 KEY|5                                  MOTION_ADD|mei|action5|Motion\mei_point\mei_point_left_buttom.vmd|FULL|ONCE
#1 1 KEY|6                                  MOTION_ADD|mei|action6|Motion\mei_point\mei_point_left_center.vmd|FULL|ONCE
#1 1 KEY|7                                  MOTION_ADD|mei|action7|Motion\mei_point\mei_point_lef_top.vmd|FULL|ONCE
#1 1 KEY|8                                  MOTION_ADD|mei|action8|Motion\mei_flash\mei_flash.vmd|FULL|ONCE              #�u�Ђ�߂����I�v�݂����Ȃ���
#1 1 KEY|9                                  MOTION_ADD|mei|action9|Motion\mei_hand_shake\mei_hand_shake.vmd|FULL|ONCE    #�΂��΂��݂����Ȃ���

#1 1 KEY|1                                  MOTION_ADD|mei|action1|Motion\mei_look\mei_look_down.vmd|FULL|ONCE
#1 1 KEY|2                                  MOTION_ADD|mei|action2|Motion\mei_self_introduction\mei_self_introduction.vmd|FULL|ONCE




