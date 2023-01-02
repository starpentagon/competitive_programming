import sys

argv = sys.argv

if len(argv) != 2:
   print('Usage: python make_snippets (header file)')
   exit(0)

with open(argv[1]) as f:
   lines = f.readlines()

   snippets = {}
   prefix = {}

   key = ''

   for line in lines:
      line = line.replace('\n', '')
      
      if '[Start]' in line:
         line = line.replace('[Start]', '')
         line = line.replace(' ', '')
         line = line.replace('//', '')

         key = line
         continue

      if '[End]' in line:
         key = ''
         continue

      if '[Prefix]' in line:
         line = line.replace('// ', '')
         line = line.replace(' ', '')
         prefix[key] = line.replace('[Prefix]', '')
         continue

      if '[Skip]' in line:
         continue

      if '[Verified]' in line:
         continue

      if key != '':
         if key not in snippets.keys():
            snippets[key] = []

         snippets[key].append(line)

print('{')

for key in snippets.keys():
   print('"{}": {{'.format(key))

   print('\t"prefix": "{}",'.format(prefix[key]))

   print('\t"body": [')

   for l in snippets[key]:
      print('\t\t"{}",'.format(l.replace('"', '\\"')))

   print('\t],')
   print('\t"description": "{}"'.format(key))
   print('},')

print('}')


