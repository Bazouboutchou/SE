// https://www.blaess.fr/christophe/2011/10/09/efficacite-des-ipc-semaphore-et-memoire-partagee/
#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/mman.h>
#include <string.h>

int* partage; // donné partagé entre les threads

/* 
 * @brief : thread qui affiche les nombres pair en recuperant
 * 					la valeur dans la donnee partagé
 */
void* threadA(){
	do{
		sleep(1);

		if (*partage % 2 == 0) // si la donnée est pair
			printf("Thread A pair : %d \n", *partage);
	}while (*partage != 1000);

}

/* 
 * @brief : thread qui affiche les nombres impair en recuperant
 * 					la valeur dans la donnee partagé
 */
void* threadB(){	
	do{
		sleep(1);

		if (*partage % 2 != 0) // si la donnée est impair
			printf("Thread B impair : %d\n", *partage);
	}while (*partage != 1000);

}

int main (){
	pthread_t threada;
	pthread_t threadb;
	int fd; 
	int i;	

	fd = shm_open("partage.mem", O_RDWR | O_CREAT, 0600); // creer un segment memoire en lecture ecriture 
	if (fd == -1){
		perror("shm_open");
		exit(1);	
	}
	if ((ftruncate(fd, 4096)) == -1){ //  tronquent le fichier référencé par path ou par le descripteur fd à une longueur d'exactement length octets.


		perror("ftruncate failure");
		exit(0);
	}
	
	//  crée une nouvelle projection dans l'espace d'adressage virtuel du processus appelant. la vaiable partage est mappé en memoire protegé en lecture et ecriture 
	partage = (int*) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (partage == MAP_FAILED){
		perror("mmap");
		exit(1);	
	}	
	// creer le thread pair
	if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
	}
	
		// creer le thread impair
	if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
	}		
	
	// ecrit i dans la memoire partagé
	for(i = 0; i <= 1000; ++i){
		memcpy(partage, &i, sizeof(int));
		sleep(1);
	}
	
	// attend les threads
	pthread_join(threada, NULL);

	pthread_join(threadb, NULL);
}


/*
 * trace : 
Thread B impair : 1
Thread A pair : 2 
Thread B impair : 3
Thread A pair : 4 
Thread B impair : 5
Thread A pair : 6 
Thread B impair : 7
Thread A pair : 8 
Thread B impair : 9
Thread A pair : 10 
Thread B impair : 11
Thread A pair : 12 
Thread B impair : 13
Thread A pair : 14 
Thread B impair : 15
Thread A pair : 16 
Thread B impair : 17
Thread A pair : 18 
Thread B impair : 19
Thread A pair : 20 
Thread B impair : 21
Thread A pair : 22 
Thread B impair : 23
Thread A pair : 24 
Thread B impair : 25
Thread A pair : 26 
Thread B impair : 27
Thread A pair : 28 
Thread B impair : 29
Thread A pair : 30 
Thread B impair : 31
Thread A pair : 32 
Thread B impair : 33
Thread A pair : 34 
Thread B impair : 35
Thread A pair : 36 
Thread B impair : 37
Thread A pair : 38 
Thread B impair : 39
Thread A pair : 40 
Thread B impair : 41
Thread A pair : 42 
Thread B impair : 43
Thread A pair : 44 
Thread B impair : 45
Thread A pair : 46 
Thread B impair : 47
Thread A pair : 48 
Thread B impair : 49
Thread A pair : 50 
Thread B impair : 51
Thread A pair : 52 
Thread B impair : 53
Thread A pair : 54 
Thread B impair : 55
Thread A pair : 56 
Thread B impair : 57
Thread A pair : 58 
Thread B impair : 59
Thread A pair : 60 
Thread B impair : 61
Thread A pair : 62 
Thread B impair : 63
Thread A pair : 64 
Thread B impair : 65
Thread A pair : 66 
Thread B impair : 67
Thread A pair : 68 
Thread B impair : 69
Thread A pair : 70 
Thread B impair : 71
Thread A pair : 72 
Thread B impair : 73
Thread A pair : 74 
Thread B impair : 75
Thread A pair : 76 
Thread B impair : 77
Thread A pair : 78 
Thread B impair : 79
Thread A pair : 80 
Thread B impair : 81
Thread A pair : 82 
Thread B impair : 83
Thread A pair : 84 
Thread B impair : 85
Thread A pair : 86 
Thread B impair : 87
Thread A pair : 88 
Thread B impair : 89
Thread A pair : 90 
Thread B impair : 91
Thread A pair : 92 
Thread B impair : 93
Thread A pair : 94 
Thread B impair : 95
Thread A pair : 96 
Thread B impair : 97
Thread A pair : 98 
Thread B impair : 99
Thread A pair : 100 
Thread B impair : 101
Thread A pair : 102 
Thread B impair : 103
Thread A pair : 104 
Thread B impair : 105
Thread A pair : 106 
Thread B impair : 107
Thread A pair : 108 
Thread B impair : 109
Thread A pair : 110 
Thread B impair : 111
Thread A pair : 112 
Thread B impair : 113
Thread A pair : 114 
Thread B impair : 115
Thread A pair : 116 
Thread B impair : 117
Thread A pair : 118 
Thread B impair : 119
Thread A pair : 120 
Thread B impair : 121
Thread A pair : 122 
Thread B impair : 123
Thread A pair : 124 
Thread B impair : 125
Thread A pair : 126 
Thread B impair : 127
Thread A pair : 128 
Thread B impair : 129
Thread A pair : 130 
Thread B impair : 131
Thread A pair : 132 
Thread B impair : 133
Thread A pair : 134 
Thread B impair : 135
Thread A pair : 136 
Thread B impair : 137
Thread A pair : 138 
Thread B impair : 139
Thread A pair : 140 
Thread B impair : 141
Thread A pair : 142 
Thread B impair : 143
Thread A pair : 144 
Thread B impair : 145
Thread A pair : 146 
Thread B impair : 147
Thread A pair : 148 
Thread B impair : 149
Thread A pair : 150 
Thread B impair : 151
Thread A pair : 152 
Thread B impair : 153
Thread A pair : 154 
Thread B impair : 155
Thread A pair : 156 
Thread B impair : 157
Thread A pair : 158 
Thread B impair : 159
Thread A pair : 160 
Thread B impair : 161
Thread A pair : 162 
Thread B impair : 163
Thread A pair : 164 
Thread B impair : 165
Thread A pair : 166 
Thread B impair : 167
Thread A pair : 168 
Thread B impair : 169
Thread A pair : 170 
Thread B impair : 171
Thread A pair : 172 
Thread B impair : 173
Thread A pair : 174 
Thread B impair : 175
Thread A pair : 176 
Thread B impair : 177
Thread A pair : 178 
Thread B impair : 179
Thread A pair : 180 
Thread B impair : 181
Thread A pair : 182 
Thread B impair : 183
Thread A pair : 184 
Thread B impair : 185
Thread A pair : 186 
Thread B impair : 187
Thread A pair : 188 
Thread B impair : 189
Thread A pair : 190 
Thread B impair : 191
Thread A pair : 192 
Thread B impair : 193
Thread A pair : 194 
Thread B impair : 195
Thread A pair : 196 
Thread B impair : 197
Thread A pair : 198 
Thread B impair : 199
Thread A pair : 200 
Thread B impair : 201
Thread A pair : 202 
Thread B impair : 203
Thread A pair : 204 
Thread B impair : 205
Thread A pair : 206 
Thread B impair : 207
Thread A pair : 208 
Thread B impair : 209
Thread A pair : 210 
Thread B impair : 211
Thread A pair : 212 
Thread B impair : 213
Thread A pair : 214 
Thread B impair : 215
Thread A pair : 216 
Thread B impair : 217
Thread A pair : 218 
Thread B impair : 219
Thread A pair : 220 
Thread B impair : 221
Thread A pair : 222 
Thread B impair : 223
Thread A pair : 224 
Thread B impair : 225
Thread A pair : 226 
Thread B impair : 227
Thread A pair : 228 
Thread B impair : 229
Thread A pair : 230 
Thread B impair : 231
Thread A pair : 232 
Thread B impair : 233
Thread A pair : 234 
Thread B impair : 235
Thread A pair : 236 
Thread B impair : 237
Thread A pair : 238 
Thread B impair : 239
Thread A pair : 240 
Thread B impair : 241
Thread A pair : 242 
Thread B impair : 243
Thread A pair : 244 
Thread B impair : 245
Thread A pair : 246 
Thread B impair : 247
Thread A pair : 248 
Thread B impair : 249
Thread A pair : 250 
Thread B impair : 251
Thread A pair : 252 
Thread B impair : 253
Thread A pair : 254 
Thread B impair : 255
Thread A pair : 256 
Thread B impair : 257
Thread A pair : 258 
Thread B impair : 259
Thread A pair : 260 
Thread B impair : 261
Thread A pair : 262 
Thread B impair : 263
Thread A pair : 264 
Thread B impair : 265
Thread A pair : 266 
Thread B impair : 267
Thread A pair : 268 
Thread B impair : 269
Thread A pair : 270 
Thread B impair : 271
Thread A pair : 272 
Thread B impair : 273
Thread A pair : 274 
Thread B impair : 275
Thread A pair : 276 
Thread B impair : 277
Thread A pair : 278 
Thread B impair : 279
Thread A pair : 280 
Thread B impair : 281
Thread A pair : 282 
Thread B impair : 283
Thread A pair : 284 
Thread B impair : 285
Thread A pair : 286 
Thread B impair : 287
Thread A pair : 288 
Thread B impair : 289
Thread A pair : 290 
Thread B impair : 291
Thread A pair : 292 
Thread B impair : 293
Thread A pair : 294 
Thread B impair : 295
Thread A pair : 296 
Thread B impair : 297
Thread A pair : 298 
Thread B impair : 299
Thread A pair : 300 
Thread B impair : 301
Thread A pair : 302 
Thread B impair : 303
Thread A pair : 304 
Thread B impair : 305
Thread A pair : 306 
Thread B impair : 307
Thread A pair : 308 
Thread B impair : 309
Thread A pair : 310 
Thread B impair : 311
Thread A pair : 312 
Thread B impair : 313
Thread A pair : 314 
Thread B impair : 315
Thread A pair : 316 
Thread B impair : 317
Thread A pair : 318 
Thread B impair : 319
Thread A pair : 320 
Thread B impair : 321
Thread A pair : 322 
Thread B impair : 323
Thread A pair : 324 
Thread B impair : 325
Thread A pair : 326 
Thread B impair : 327
Thread A pair : 328 
Thread B impair : 329
Thread A pair : 330 
Thread B impair : 331
Thread A pair : 332 
Thread B impair : 333
Thread A pair : 334 
Thread B impair : 335
Thread A pair : 336 
Thread B impair : 337
Thread A pair : 338 
Thread B impair : 339
Thread A pair : 340 
Thread B impair : 341
Thread A pair : 342 
Thread B impair : 343
Thread A pair : 344 
Thread B impair : 345
Thread A pair : 346 
Thread B impair : 347
Thread A pair : 348 
Thread B impair : 349
Thread A pair : 350 
Thread B impair : 351
Thread A pair : 352 
Thread B impair : 353
Thread A pair : 354 
Thread B impair : 355
Thread A pair : 356 
Thread B impair : 357
Thread A pair : 358 
Thread B impair : 359
Thread A pair : 360 
Thread B impair : 361
Thread A pair : 362 
Thread B impair : 363
Thread A pair : 364 
Thread B impair : 365
Thread A pair : 366 
Thread B impair : 367
Thread A pair : 368 
Thread B impair : 369
Thread A pair : 370 
Thread B impair : 371
Thread A pair : 372 
Thread B impair : 373
Thread A pair : 374 
Thread B impair : 375
Thread A pair : 376 
Thread B impair : 377
Thread A pair : 378 
Thread B impair : 379
Thread A pair : 380 
Thread B impair : 381
Thread A pair : 382 
Thread B impair : 383
Thread A pair : 384 
Thread B impair : 385
Thread A pair : 386 
Thread B impair : 387
Thread A pair : 388 
Thread B impair : 389
Thread A pair : 390 
Thread B impair : 391
Thread A pair : 392 
Thread B impair : 393
Thread A pair : 394 
Thread B impair : 395
Thread A pair : 396 
Thread B impair : 397
Thread A pair : 398 
Thread B impair : 399
Thread A pair : 400 
Thread B impair : 401
Thread A pair : 402 
Thread B impair : 403
Thread A pair : 404 
Thread B impair : 405
Thread A pair : 406 
Thread B impair : 407
Thread A pair : 408 
Thread B impair : 409
Thread A pair : 410 
Thread B impair : 411
Thread A pair : 412 
Thread B impair : 413
Thread A pair : 414 
Thread B impair : 415
Thread A pair : 416 
Thread B impair : 417
Thread A pair : 418 
Thread B impair : 419
Thread A pair : 420 
Thread B impair : 421
Thread A pair : 422 
Thread B impair : 423
Thread A pair : 424 
Thread B impair : 425
Thread A pair : 426 
Thread B impair : 427
Thread A pair : 428 
Thread B impair : 429
Thread A pair : 430 
Thread B impair : 431
Thread A pair : 432 
Thread B impair : 433
Thread A pair : 434 
Thread B impair : 435
Thread A pair : 436 
Thread B impair : 437
Thread A pair : 438 
Thread B impair : 439
Thread A pair : 440 
Thread B impair : 441
Thread A pair : 442 
Thread B impair : 443
Thread A pair : 444 
Thread B impair : 445
Thread A pair : 446 
Thread B impair : 447
Thread A pair : 448 
Thread B impair : 449
Thread A pair : 450 
Thread B impair : 451
Thread A pair : 452 
Thread B impair : 453
Thread A pair : 454 
Thread B impair : 455
Thread A pair : 456 
Thread B impair : 457
Thread A pair : 458 
Thread B impair : 459
Thread A pair : 460 
Thread B impair : 461
Thread A pair : 462 
Thread B impair : 463
Thread A pair : 464 
Thread B impair : 465
Thread A pair : 466 
Thread B impair : 467
Thread A pair : 468 
Thread B impair : 469
Thread A pair : 470 
Thread B impair : 471
Thread A pair : 472 
Thread B impair : 473
Thread A pair : 474 
Thread B impair : 475
Thread A pair : 476 
Thread B impair : 477
Thread A pair : 478 
Thread B impair : 479
Thread A pair : 480 
Thread B impair : 481
Thread A pair : 482 
Thread B impair : 483
Thread A pair : 484 
Thread B impair : 485
Thread A pair : 486 
Thread B impair : 487
Thread A pair : 488 
Thread B impair : 489
Thread A pair : 490 
Thread B impair : 491
Thread A pair : 492 
Thread B impair : 493
Thread A pair : 494 
Thread B impair : 495
Thread A pair : 496 
Thread B impair : 497
Thread A pair : 498 
Thread B impair : 499
Thread A pair : 500 
Thread B impair : 501
Thread A pair : 502 
Thread B impair : 503
Thread A pair : 504 
Thread B impair : 505
Thread A pair : 506 
Thread B impair : 507
Thread A pair : 508 
Thread B impair : 509
Thread A pair : 510 
Thread B impair : 511
Thread A pair : 512 
Thread B impair : 513
Thread A pair : 514 
Thread B impair : 515
Thread A pair : 516 
Thread B impair : 517
Thread A pair : 518 
Thread B impair : 519
Thread A pair : 520 
Thread B impair : 521
Thread A pair : 522 
Thread B impair : 523
Thread A pair : 524 
Thread B impair : 525
Thread A pair : 526 
Thread B impair : 527
Thread A pair : 528 
Thread B impair : 529
Thread A pair : 530 
Thread B impair : 531
Thread A pair : 532 
Thread B impair : 533
Thread A pair : 534 
Thread B impair : 535
Thread A pair : 536 
Thread B impair : 537
Thread A pair : 538 
Thread B impair : 539
Thread A pair : 540 
Thread B impair : 541
Thread A pair : 542 
Thread B impair : 543
Thread A pair : 544 
Thread B impair : 545
Thread A pair : 546 
Thread B impair : 547
Thread A pair : 548 
Thread B impair : 549
Thread A pair : 550 
Thread B impair : 551
Thread A pair : 552 
Thread B impair : 553
Thread A pair : 554 
Thread B impair : 555
Thread A pair : 556 
Thread B impair : 557
Thread A pair : 558 
Thread B impair : 559
Thread A pair : 560 
Thread B impair : 561
Thread A pair : 562 
Thread B impair : 563
Thread A pair : 564 
Thread B impair : 565
Thread A pair : 566 
Thread B impair : 567
Thread A pair : 568 
Thread B impair : 569
Thread A pair : 570 
Thread B impair : 571
Thread A pair : 572 
Thread B impair : 573
Thread A pair : 574 
Thread B impair : 575
Thread A pair : 576 

etc jusqua 1000
*/

