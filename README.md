# Introduction to Real Time Operating Systems (RTOS)

Welcome to the demo code and solutions section of my Introduction to RTOS course! This repository houses all of the example code and solutions that you may use as references when working through the RTOS examples for FreeRTOS. 

![Intro to RTOS course logo](https://raw.githubusercontent.com/ShawnHymel/introduction-to-rtos/main/images/intro-to-rtos.png)

This course is hosted on YouTube that you may take for free. All you need to do is watch the videos and complete the challenge issued at the end of each video. I highly recommend you try each challenge before peeking at the solutions here. The PowerPoint slides are also made available (under the CC BY 4.0 license) if you wish to use or modify them for review or teaching your own RTOS class.

| Chapter | Title | Video | Solution | Slides |
|---------|-------|-------|----------|--------|
| 01 | Getting Started with FreeRTOS | [[video]](https://www.youtube.com/watch?v=F321087yYy4&list=PLEBQazB0HUyQ4hAPU1cJED6t3DU0h34bz) | [[guide]](https://www.digikey.com/en/maker/projects/what-is-a-realtime-operating-system-rtos/28d8087f53844decafa5000d89608016) | [[slides]]() |

The first video in the series is found here: [Introduction to RTOS Part 1 - What is a Real-Time Operating System (RTOS)?](https://www.youtube.com/watch?v=F321087yYy4)

Written guides that explain the solutions can be found [among my maker.io projects](https://www.digikey.com/en/maker/profiles/72825bdd887a427eaf8d960b6505adac).

## Directory Structure

Examples and solutions are housed in dirctories that correspond to each chapter or video number. For example, if you watch "Intro to RTOS Part 3 - Task Scheduling," you should refer to the directory *03-task-scheduling-and-management.* In it, you will find 2 directories, one marked "demo" that gives the finished demo code used during the video (so you may run it and examine it at your own pace) and another marked "solution" that provides one possible solution to the challenge issued at the end of the video.

If a challenge is issued in a video that starts with some code, it will be listed as a *challenge* Arduino sketch in the naming scheme shown below.

Directories are in the following structure (where xx is the part or chapter number):

```
xx-<name of chapter>
|- esp32-freertos-xx-challenge-<name>
|- esp32-freertos-xx-demo-<name>
|- esp32-freertos-xx-solution-<name>
```

The only exception to this is the *images* directory, which is where I keep images for this repository.

PowerPoint slides used in each video can be found within the respective *xx-<name of chapter>* directory.

## License

PowerPoint slides are licensed under the [Creative Commons CC BY 4.0 license](https://creativecommons.org/licenses/by/4.0/).

All code in this repository, unless otherwise noted, is licensed under the [Zero-Clause BSD / Free Public License 1.0.0 (0BSD)](https://opensource.org/licenses/0BSD).

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.