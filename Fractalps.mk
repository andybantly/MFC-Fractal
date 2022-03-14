
Fractalps.dll: dlldata.obj Fractal_p.obj Fractal_i.obj
	link /dll /out:Fractalps.dll /def:Fractalps.def /entry:DllMain dlldata.obj Fractal_p.obj Fractal_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Fractalps.dll
	@del Fractalps.lib
	@del Fractalps.exp
	@del dlldata.obj
	@del Fractal_p.obj
	@del Fractal_i.obj
