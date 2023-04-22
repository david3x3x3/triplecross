# this python script works, but it's about 30x slower than the C version.

import sys, time, os
import numpy as np
from itertools import permutations

#    ab
#cdefghi
#jklmnop
# qr

mvnames = ['rucd', 'lucd', 'urdc', 'uldc']
mvseq0 = [['cdelrqj', 'bafmnoh'],
         ['fedkqrm', 'abipong'],
         ['jqrledc', 'honmfab'],
         ['mrqkdef', 'gnopiba']]

def prt(s):
    print('    '+s[:2])
    print(s[2:9])
    print(s[9:16])
    print(' '+s[16:])
    print('')

def move0(s, n):
    s2 = list(s)
    for i in range(7):
        for j in range(2):
            s2[ord(mvseq0[n][j][(i+1)%7])-ord('a')] = \
                s[ord(mvseq0[n][j][(i)%7])-ord('a')]
    return s2

def move(s, n):
    #res = ''
    #for i in mvseq[n]:
    #    res += s[i]
    #return res
    return [s[i] for i in mvseq[n]]

mvseq = []
for i in range(4):
    mvseq += [move0(range(18), i)]

start = '..---..12.....34ba'

def mycmp(a, b):
    if a<b:
        return -1
    elif a>b:
        return 1
    return 0

def next_permutation(seq, pred=mycmp):
    """Like C++ std::next_permutation() but implemented as
    generator. Yields copies of seq."""

    def reverse(seq, start, end):
        # seq = seq[:start] + reversed(seq[start:end]) + \
        #       seq[end:]
        end -= 1
        if end <= start:
            return
        while True:
            seq[start], seq[end] = seq[end], seq[start]
            if start == end or start+1 == end:
                return
            start += 1
            end -= 1
    
    if not seq:
        raise StopIteration

    try:
        seq[0]
    except TypeError:
        raise TypeError("seq must allow random access.")

    first = 0
    last = len(seq)
    seq = seq[:]

    # Yield input sequence as the STL version is often
    # used inside do {} while.
    yield seq
    
    if last == 1:
        raise StopIteration

    while True:
        next = last - 1

        while True:
            # Step 1.
            next1 = next
            next -= 1
            
            if pred(seq[next], seq[next1]) < 0:
                # Step 2.
                mid = last - 1
                while not (pred(seq[next], seq[mid]) < 0):
                    mid -= 1
                seq[next], seq[mid] = seq[mid], seq[next]
                
                # Step 3.
                reverse(seq, next1, last)

                # Change to yield references to get rid of
                # (at worst) |seq|! copy operations.
                yield seq[:]
                break
            if next == first:
                return
                raise StopIteration
    raise StopIteration

def all_perms(s, permin, permout):
    res = []
    num = 0
    for p in next_permutation(s):
        v = 0
        for v2 in tuple(p):
            v *= 2
            if v2:
                v += 1
        #permin[tuple(p)] = num
        #permout[num] = tuple(p)
        permin[v] = num
        permout[num] = v
        num += 1

perma2num = {}
num2perma = {}
all_perms([False]*9+[True]*9, perma2num, num2perma)

permb2num = {}
num2permb = {}
all_perms([False]*3+[True]*6, permb2num, num2permb)

permc2num = {}
num2permc = {}
for i, p in enumerate(permutations('1234ab')):
    permc2num[''.join(p)] = i
    num2permc[i] = ''.join(p)

# for k in perma2num:
#     print(f'{k:b} - {perma2num[k]}')
# exit(0)

#print('\n'.join(map(str,perma2num.keys())))
#print('\n'.join(map(str,permb2num.keys())))
#print('\n'.join(map(str,permc2num.keys())))

counta = len(perma2num)
countb = len(permb2num)
countc = len(permc2num)
print(counta, countb, countc, counta*countb*countc)
distsize = counta*countb*countc
print(f'distsize = {distsize}')
dists = np.zeros((distsize//4,), dtype=np.uint8)

def distget(n):
    return 0b11 & (dists[n//4]>>((n%4)*2))

def distput(n, newval):
    global dists
    n1 = n//4
    n2 = n%4
    dists[n1] = ((0b11111111^(0b11<<(2*n2))) & dists[n1]) ^ (newval<<(2*n2))

def pos2num(s):
    #print(f'pos2num{s}')
    a = 0
    b = 0
    c = ''
    for ch in s:
        if ch == '.':
            #a += (False,)
            a = a*2
        elif ch == '-':
            #a += (True,)
            a = a*2 + 1
            #b += (False,)
            b = b*2
        else:
            #a += (True,)
            a = a*2 + 1
            #b += (True,)
            b = b*2 + 1
            c += ch
    #print(f'a = {a:b}')
    #print(f'b = {b:b}')
    #print(f'c = {c}', flush=True)
    return (perma2num[a]*84+permb2num[b])*720+permc2num[c] 

def num2pos(n):
    c = num2permc[n%720]
    n //= 720
    b = num2permb[n%84]
    n //= 84
    a = num2perma[n]
    res = ''
    bn = 0
    cn = 0
    for i in range(18):
        if (1<<i) & a != 0:
            if (1<<bn) & b != 0:
                res += c[cn]
                cn += 1
            else:
                res += '-'
            bn += 1
        else:
            res += '.'
    return res

def time_fmt(time_in):
    time_in = int(time_in)
    sec_left = time_in % 60
    time_in //= 60
    min_left = time_in % 60
    time_in //= 60
    hr_left = time_in % 60
    return f'{hr_left}:{min_left:02d}:{sec_left:02d}'

sn = pos2num(start)
start_time = time.time() - 1
distput(sn, 1)
fpout = open('prev0.txt', 'w')
fpout.write(f'{sn}\n')
fpout.close()

savefile = 'dists.npy'
if os.path.isfile(savefile):
    dists = np.load(savefile)
else:
    depth = 1
    total = 0
    while True:
        count = 0
        fpout = open(f'prev{(depth)%2}.txt', 'w')
        for line in open(f'prev{(depth+1)%2}.txt','r'):
            pos1 = num2pos(int(line.strip()))
            for i in range(4):
                num2 = pos2num(move(pos1, i))
                if distget(num2) != 0:
                    continue
                distput(num2, depth%3+1)
                fpout.write(f'{num2}\n')
                count += 1
        fpout.close()
        total += count
        time_left = ((distsize-total)*(time.time()-start_time))/total
        print(f'{depth:2d}: {count:10d} {total:10d} {total*100/distsize:7.3f} '
              f'{time_fmt(time.time()-start_time)} {time_fmt(time_left)}',
              flush=True)
        if count == 0:
            break
        depth += 1
    print('saving data', flush=True)
    np.save(savefile, dists)

def solve(num):
    startnum = pos2num(start)
    d = distget(num)
    while num != startnum:
        pos = num2pos(num)
        for i in range(4):
            num2 = pos2num(move(pos,i))
            d2 = distget(num2)
            if (None,3,1,2)[d] == d2:
                print(f'{mvnames[i]} ', end='', flush=True)
                num = num2
                d = d2
                break
    print('')
    print('')

if len(sys.argv) > 1:
    #num = pos2num('..a..-.12--.b.34..')
    solve(pos2num(sys.argv[1]))
else:
    print('the 7 hardest triplecross starting positions:')
    for num in (302372969, 1113063043, 1368550717, 1367705187, 1410759325, 1417220809, 1743855286):
        prt(num2pos(num))
        solve(num)
    
