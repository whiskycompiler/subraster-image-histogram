# Subraster Image Histogram Library

## What is this?
<p>A small library to get a histogram of an image using subrasters.
Instead of just binning all color values over the entire image, it instead bins them by "sector" as well. This can be helpful if you want to do calculations using different weights on different areas of the image.</p>
<br/>

## How to build it?
### Preconditions
- install CMAKE (can be installed together with VS 2022)
- install OpenCV SDK (used by the lib to read the image)
### Build and install
- use VS 2022 or use cmake commands directly

<br/>

## How to use it?
### In C++
Link it like any other lib.

### In C#
<p>
DllImport it using the entrypoint names aquired with the VS developer console <code>dumpbin /export HistogramLib.dll</code> command.
</p>

Example:
```C#
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
private delegate void Callback(IntPtr data, int length);

[DllImport("HistogramLib.dll",
    EntryPoint = "?getFullSubrasterHistogramFromFile@@YAXPEBDIHP6AXPEAHH@Z@Z",
    CharSet = CharSet.Ansi)]
static extern void GetFullSubrasterHistogramFromFile(
    [MarshalAs(UnmanagedType.LPStr)] string filePath,
    uint bucketCount,
    int rasterSideLength,
    Callback resultCallback);
```
This uses a callback so that we do not need to worry about freeing the memory on the managed side. Just use <code>Marshal.Copy(IntPtr, int[], int, int)</code> to copy the data provided by the callback into managed space. After the callback is processed memory will be freed by the native side.
