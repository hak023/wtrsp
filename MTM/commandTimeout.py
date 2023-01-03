import sys
import subprocess

if __name__ == "__main__":   
   cmd = list()
   for v in range(2, len(sys.argv)):
      #print(sys.argv[v])
      cmd.append(sys.argv[v])

   #print(cmd)
   #a="ffmpeg -i /home/ibc/music_original5/vwtrss/audio/01000034289719.mp3 -f mp3 -acodec mp3 -ac 2 -ar 44100 -ab 192000 -y /home/ibc/music/mig/vwtrss/test/MP3TestFile_1.mp3"
   #cmd=a.split()
   #print(a.split())
   #proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
   #print("timeout:" + sys.argv[1])
   proc = subprocess.Popen(cmd)
   try:
      #outs, errs = proc.communicate(timeout=60)
      #outs, errs = proc.communicate(timeout=int(sys.argv[1]))
      proc.communicate(timeout=int(sys.argv[1]))
      proc.terminate()
   except subprocess.TimeoutExpired:
      proc.kill()
      #outs, errs = proc.communicate()
      print('Exiting normally, received signal 15.')

   #proc.terminate()
