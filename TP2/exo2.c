#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t mutexA;
sem_t mutexB;

/* 
 * @brief : thread qui affiche les nombres pair 10 par 10. Ici i est la donnee
 *          critique, il faudra donc apres avoir afficher les 10 entiers 
 *          bloque le thread et debloquer l'autre 
 */
void* threadA(){
	int i;	
	int cpt = 0; // pour l'affichage 10 par 10
	sem_wait(&mutexA);	// le thread attend dirrectement
 
	for (i = 1; i <= 1000; ++i){
 		
		if (i % 2 == 0){ // si pair
			printf("Thread A affichage pair : ");

			printf(" %d ", i);
			printf("\n");
			++cpt;
		}
		if(cpt == 10){
			printf("\n");		
			sem_post(&mutexB);	// on libere l'autre thread
			sem_wait(&mutexA); // on bloque ce thread

			cpt = 0; 
		}
				
	}
}

/* 
 * @brief : thread qui affiche les nombres impair 10 par 10. Ici i est la donnee
 *          critique, il faudra donc apres avoir afficher les 10 entiers 
 *          bloque le thread et debloquer l'autre 
 */
void* threadB(){
	int i;	

	int cpt = 0; // pour l'affichage 10 par 10
 
	for (i = 1; i <= 1000; ++i){
		if (i % 2 != 0){ // impair
			printf("Thread B affichage impair : ");

			printf(" %d ", i);
			printf("\n");
			++cpt;
		}
		if(cpt == 10){
			printf("\n");
			sem_post(&mutexA);	// on libere l'autre thread
	  	sem_wait(&mutexB);	// on bloque ce thread

			cpt = 0; 
		}
	}
	sem_post(&mutexA); // on le libere de nouveau sinon on bloque infiniment
}

int main (){
	pthread_t threada;
	pthread_t threadb;
	// initialisation des sémaphore à 0
	sem_init(&mutexA, 0, 0);    
	sem_init(&mutexB, 0, 0);    
	
	// creer les threads
  if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
  }
	if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
  }

	// attend les threads
	pthread_join(threadb, NULL);
	pthread_join(threada, NULL);
}

/* trace :
Thread B affichage impair :  1 
Thread B affichage impair :  3 
Thread B affichage impair :  5 
Thread B affichage impair :  7 
Thread B affichage impair :  9 
Thread B affichage impair :  11 
Thread B affichage impair :  13 
Thread B affichage impair :  15 
Thread B affichage impair :  17 
Thread B affichage impair :  19 

Thread A affichage pair :  2 
Thread A affichage pair :  4 
Thread A affichage pair :  6 
Thread A affichage pair :  8 
Thread A affichage pair :  10 
Thread A affichage pair :  12 
Thread A affichage pair :  14 
Thread A affichage pair :  16 
Thread A affichage pair :  18 
Thread A affichage pair :  20 

Thread B affichage impair :  21 
Thread B affichage impair :  23 
Thread B affichage impair :  25 
Thread B affichage impair :  27 
Thread B affichage impair :  29 
Thread B affichage impair :  31 
Thread B affichage impair :  33 
Thread B affichage impair :  35 
Thread B affichage impair :  37 
Thread B affichage impair :  39 

Thread A affichage pair :  22 
Thread A affichage pair :  24 
Thread A affichage pair :  26 
Thread A affichage pair :  28 
Thread A affichage pair :  30 
Thread A affichage pair :  32 
Thread A affichage pair :  34 
Thread A affichage pair :  36 
Thread A affichage pair :  38 
Thread A affichage pair :  40 

Thread B affichage impair :  41 
Thread B affichage impair :  43 
Thread B affichage impair :  45 
Thread B affichage impair :  47 
Thread B affichage impair :  49 
Thread B affichage impair :  51 
Thread B affichage impair :  53 
Thread B affichage impair :  55 
Thread B affichage impair :  57 
Thread B affichage impair :  59 

Thread A affichage pair :  42 
Thread A affichage pair :  44 
Thread A affichage pair :  46 
Thread A affichage pair :  48 
Thread A affichage pair :  50 
Thread A affichage pair :  52 
Thread A affichage pair :  54 
Thread A affichage pair :  56 
Thread A affichage pair :  58 
Thread A affichage pair :  60 

Thread B affichage impair :  61 
Thread B affichage impair :  63 
Thread B affichage impair :  65 
Thread B affichage impair :  67 
Thread B affichage impair :  69 
Thread B affichage impair :  71 
Thread B affichage impair :  73 
Thread B affichage impair :  75 
Thread B affichage impair :  77 
Thread B affichage impair :  79 

Thread A affichage pair :  62 
Thread A affichage pair :  64 
Thread A affichage pair :  66 
Thread A affichage pair :  68 
Thread A affichage pair :  70 
Thread A affichage pair :  72 
Thread A affichage pair :  74 
Thread A affichage pair :  76 
Thread A affichage pair :  78 
Thread A affichage pair :  80 

Thread B affichage impair :  81 
Thread B affichage impair :  83 
Thread B affichage impair :  85 
Thread B affichage impair :  87 
Thread B affichage impair :  89 
Thread B affichage impair :  91 
Thread B affichage impair :  93 
Thread B affichage impair :  95 
Thread B affichage impair :  97 
Thread B affichage impair :  99 

Thread A affichage pair :  82 
Thread A affichage pair :  84 
Thread A affichage pair :  86 
Thread A affichage pair :  88 
Thread A affichage pair :  90 
Thread A affichage pair :  92 
Thread A affichage pair :  94 
Thread A affichage pair :  96 
Thread A affichage pair :  98 
Thread A affichage pair :  100 

Thread B affichage impair :  101 
Thread B affichage impair :  103 
Thread B affichage impair :  105 
Thread B affichage impair :  107 
Thread B affichage impair :  109 
Thread B affichage impair :  111 
Thread B affichage impair :  113 
Thread B affichage impair :  115 
Thread B affichage impair :  117 
Thread B affichage impair :  119 

Thread A affichage pair :  102 
Thread A affichage pair :  104 
Thread A affichage pair :  106 
Thread A affichage pair :  108 
Thread A affichage pair :  110 
Thread A affichage pair :  112 
Thread A affichage pair :  114 
Thread A affichage pair :  116 
Thread A affichage pair :  118 
Thread A affichage pair :  120 

Thread B affichage impair :  121 
Thread B affichage impair :  123 
Thread B affichage impair :  125 
Thread B affichage impair :  127 
Thread B affichage impair :  129 
Thread B affichage impair :  131 
Thread B affichage impair :  133 
Thread B affichage impair :  135 
Thread B affichage impair :  137 
Thread B affichage impair :  139 

Thread A affichage pair :  122 
Thread A affichage pair :  124 
Thread A affichage pair :  126 
Thread A affichage pair :  128 
Thread A affichage pair :  130 
Thread A affichage pair :  132 
Thread A affichage pair :  134 
Thread A affichage pair :  136 
Thread A affichage pair :  138 
Thread A affichage pair :  140 

Thread B affichage impair :  141 
Thread B affichage impair :  143 
Thread B affichage impair :  145 
Thread B affichage impair :  147 
Thread B affichage impair :  149 
Thread B affichage impair :  151 
Thread B affichage impair :  153 
Thread B affichage impair :  155 
Thread B affichage impair :  157 
Thread B affichage impair :  159 

Thread A affichage pair :  142 
Thread A affichage pair :  144 
Thread A affichage pair :  146 
Thread A affichage pair :  148 
Thread A affichage pair :  150 
Thread A affichage pair :  152 
Thread A affichage pair :  154 
Thread A affichage pair :  156 
Thread A affichage pair :  158 
Thread A affichage pair :  160 

Thread B affichage impair :  161 
Thread B affichage impair :  163 
Thread B affichage impair :  165 
Thread B affichage impair :  167 
Thread B affichage impair :  169 
Thread B affichage impair :  171 
Thread B affichage impair :  173 
Thread B affichage impair :  175 
Thread B affichage impair :  177 
Thread B affichage impair :  179 

Thread A affichage pair :  162 
Thread A affichage pair :  164 
Thread A affichage pair :  166 
Thread A affichage pair :  168 
Thread A affichage pair :  170 
Thread A affichage pair :  172 
Thread A affichage pair :  174 
Thread A affichage pair :  176 
Thread A affichage pair :  178 
Thread A affichage pair :  180 

Thread B affichage impair :  181 
Thread B affichage impair :  183 
Thread B affichage impair :  185 
Thread B affichage impair :  187 
Thread B affichage impair :  189 
Thread B affichage impair :  191 
Thread B affichage impair :  193 
Thread B affichage impair :  195 
Thread B affichage impair :  197 
Thread B affichage impair :  199 

Thread A affichage pair :  182 
Thread A affichage pair :  184 
Thread A affichage pair :  186 
Thread A affichage pair :  188 
Thread A affichage pair :  190 
Thread A affichage pair :  192 
Thread A affichage pair :  194 
Thread A affichage pair :  196 
Thread A affichage pair :  198 
Thread A affichage pair :  200 

Thread B affichage impair :  201 
Thread B affichage impair :  203 
Thread B affichage impair :  205 
Thread B affichage impair :  207 
Thread B affichage impair :  209 
Thread B affichage impair :  211 
Thread B affichage impair :  213 
Thread B affichage impair :  215 
Thread B affichage impair :  217 
Thread B affichage impair :  219 

Thread A affichage pair :  202 
Thread A affichage pair :  204 
Thread A affichage pair :  206 
Thread A affichage pair :  208 
Thread A affichage pair :  210 
Thread A affichage pair :  212 
Thread A affichage pair :  214 
Thread A affichage pair :  216 
Thread A affichage pair :  218 
Thread A affichage pair :  220 

Thread B affichage impair :  221 
Thread B affichage impair :  223 
Thread B affichage impair :  225 
Thread B affichage impair :  227 
Thread B affichage impair :  229 
Thread B affichage impair :  231 
Thread B affichage impair :  233 
Thread B affichage impair :  235 
Thread B affichage impair :  237 
Thread B affichage impair :  239 

Thread A affichage pair :  222 
Thread A affichage pair :  224 
Thread A affichage pair :  226 
Thread A affichage pair :  228 
Thread A affichage pair :  230 
Thread A affichage pair :  232 
Thread A affichage pair :  234 
Thread A affichage pair :  236 
Thread A affichage pair :  238 
Thread A affichage pair :  240 

Thread B affichage impair :  241 
Thread B affichage impair :  243 
Thread B affichage impair :  245 
Thread B affichage impair :  247 
Thread B affichage impair :  249 
Thread B affichage impair :  251 
Thread B affichage impair :  253 
Thread B affichage impair :  255 
Thread B affichage impair :  257 
Thread B affichage impair :  259 

Thread A affichage pair :  242 
Thread A affichage pair :  244 
Thread A affichage pair :  246 
Thread A affichage pair :  248 
Thread A affichage pair :  250 
Thread A affichage pair :  252 
Thread A affichage pair :  254 
Thread A affichage pair :  256 
Thread A affichage pair :  258 
Thread A affichage pair :  260 

Thread B affichage impair :  261 
Thread B affichage impair :  263 
Thread B affichage impair :  265 
Thread B affichage impair :  267 
Thread B affichage impair :  269 
Thread B affichage impair :  271 
Thread B affichage impair :  273 
Thread B affichage impair :  275 
Thread B affichage impair :  277 
Thread B affichage impair :  279 

Thread A affichage pair :  262 
Thread A affichage pair :  264 
Thread A affichage pair :  266 
Thread A affichage pair :  268 
Thread A affichage pair :  270 
Thread A affichage pair :  272 
Thread A affichage pair :  274 
Thread A affichage pair :  276 
Thread A affichage pair :  278 
Thread A affichage pair :  280 

Thread B affichage impair :  281 
Thread B affichage impair :  283 
Thread B affichage impair :  285 
Thread B affichage impair :  287 
Thread B affichage impair :  289 
Thread B affichage impair :  291 
Thread B affichage impair :  293 
Thread B affichage impair :  295 
Thread B affichage impair :  297 
Thread B affichage impair :  299 

Thread A affichage pair :  282 
Thread A affichage pair :  284 
Thread A affichage pair :  286 
Thread A affichage pair :  288 
Thread A affichage pair :  290 
Thread A affichage pair :  292 
Thread A affichage pair :  294 
Thread A affichage pair :  296 
Thread A affichage pair :  298 
Thread A affichage pair :  300 

Thread B affichage impair :  301 
Thread B affichage impair :  303 
Thread B affichage impair :  305 
Thread B affichage impair :  307 
Thread B affichage impair :  309 
Thread B affichage impair :  311 
Thread B affichage impair :  313 
Thread B affichage impair :  315 
Thread B affichage impair :  317 
Thread B affichage impair :  319 

Thread A affichage pair :  302 
Thread A affichage pair :  304 
Thread A affichage pair :  306 
Thread A affichage pair :  308 
Thread A affichage pair :  310 
Thread A affichage pair :  312 
Thread A affichage pair :  314 
Thread A affichage pair :  316 
Thread A affichage pair :  318 
Thread A affichage pair :  320 

Thread B affichage impair :  321 
Thread B affichage impair :  323 
Thread B affichage impair :  325 
Thread B affichage impair :  327 
Thread B affichage impair :  329 
Thread B affichage impair :  331 
Thread B affichage impair :  333 
Thread B affichage impair :  335 
Thread B affichage impair :  337 
Thread B affichage impair :  339 

Thread A affichage pair :  322 
Thread A affichage pair :  324 
Thread A affichage pair :  326 
Thread A affichage pair :  328 
Thread A affichage pair :  330 
Thread A affichage pair :  332 
Thread A affichage pair :  334 
Thread A affichage pair :  336 
Thread A affichage pair :  338 
Thread A affichage pair :  340 

Thread B affichage impair :  341 
Thread B affichage impair :  343 
Thread B affichage impair :  345 
Thread B affichage impair :  347 
Thread B affichage impair :  349 
Thread B affichage impair :  351 
Thread B affichage impair :  353 
Thread B affichage impair :  355 
Thread B affichage impair :  357 
Thread B affichage impair :  359 

Thread A affichage pair :  342 
Thread A affichage pair :  344 
Thread A affichage pair :  346 
Thread A affichage pair :  348 
Thread A affichage pair :  350 
Thread A affichage pair :  352 
Thread A affichage pair :  354 
Thread A affichage pair :  356 
Thread A affichage pair :  358 
Thread A affichage pair :  360 

Thread B affichage impair :  361 
Thread B affichage impair :  363 
Thread B affichage impair :  365 
Thread B affichage impair :  367 
Thread B affichage impair :  369 
Thread B affichage impair :  371 
Thread B affichage impair :  373 
Thread B affichage impair :  375 
Thread B affichage impair :  377 
Thread B affichage impair :  379 

Thread A affichage pair :  362 
Thread A affichage pair :  364 
Thread A affichage pair :  366 
Thread A affichage pair :  368 
Thread A affichage pair :  370 
Thread A affichage pair :  372 
Thread A affichage pair :  374 
Thread A affichage pair :  376 
Thread A affichage pair :  378 
Thread A affichage pair :  380 

Thread B affichage impair :  381 
Thread B affichage impair :  383 
Thread B affichage impair :  385 
Thread B affichage impair :  387 
Thread B affichage impair :  389 
Thread B affichage impair :  391 
Thread B affichage impair :  393 
Thread B affichage impair :  395 
Thread B affichage impair :  397 
Thread B affichage impair :  399 

Thread A affichage pair :  382 
Thread A affichage pair :  384 
Thread A affichage pair :  386 
Thread A affichage pair :  388 
Thread A affichage pair :  390 
Thread A affichage pair :  392 
Thread A affichage pair :  394 
Thread A affichage pair :  396 
Thread A affichage pair :  398 
Thread A affichage pair :  400 

Thread B affichage impair :  401 
Thread B affichage impair :  403 
Thread B affichage impair :  405 
Thread B affichage impair :  407 
Thread B affichage impair :  409 
Thread B affichage impair :  411 
Thread B affichage impair :  413 
Thread B affichage impair :  415 
Thread B affichage impair :  417 
Thread B affichage impair :  419 

Thread A affichage pair :  402 
Thread A affichage pair :  404 
Thread A affichage pair :  406 
Thread A affichage pair :  408 
Thread A affichage pair :  410 
Thread A affichage pair :  412 
Thread A affichage pair :  414 
Thread A affichage pair :  416 
Thread A affichage pair :  418 
Thread A affichage pair :  420 

Thread B affichage impair :  421 
Thread B affichage impair :  423 
Thread B affichage impair :  425 
Thread B affichage impair :  427 
Thread B affichage impair :  429 
Thread B affichage impair :  431 
Thread B affichage impair :  433 
Thread B affichage impair :  435 
Thread B affichage impair :  437 
Thread B affichage impair :  439 

Thread A affichage pair :  422 
Thread A affichage pair :  424 
Thread A affichage pair :  426 
Thread A affichage pair :  428 
Thread A affichage pair :  430 
Thread A affichage pair :  432 
Thread A affichage pair :  434 
Thread A affichage pair :  436 
Thread A affichage pair :  438 
Thread A affichage pair :  440 

Thread B affichage impair :  441 
Thread B affichage impair :  443 
Thread B affichage impair :  445 
Thread B affichage impair :  447 
Thread B affichage impair :  449 
Thread B affichage impair :  451 
Thread B affichage impair :  453 
Thread B affichage impair :  455 
Thread B affichage impair :  457 
Thread B affichage impair :  459 

Thread A affichage pair :  442 
Thread A affichage pair :  444 
Thread A affichage pair :  446 
Thread A affichage pair :  448 
Thread A affichage pair :  450 
Thread A affichage pair :  452 
Thread A affichage pair :  454 
Thread A affichage pair :  456 
Thread A affichage pair :  458 
Thread A affichage pair :  460 

Thread B affichage impair :  461 
Thread B affichage impair :  463 
Thread B affichage impair :  465 
Thread B affichage impair :  467 
Thread B affichage impair :  469 
Thread B affichage impair :  471 
Thread B affichage impair :  473 
Thread B affichage impair :  475 
Thread B affichage impair :  477 
Thread B affichage impair :  479 

Thread A affichage pair :  462 
Thread A affichage pair :  464 
Thread A affichage pair :  466 
Thread A affichage pair :  468 
Thread A affichage pair :  470 
Thread A affichage pair :  472 
Thread A affichage pair :  474 
Thread A affichage pair :  476 
Thread A affichage pair :  478 
Thread A affichage pair :  480 

Thread B affichage impair :  481 
Thread B affichage impair :  483 
Thread B affichage impair :  485 
Thread B affichage impair :  487 
Thread B affichage impair :  489 
Thread B affichage impair :  491 
Thread B affichage impair :  493 
Thread B affichage impair :  495 
Thread B affichage impair :  497 
Thread B affichage impair :  499 

Thread A affichage pair :  482 
Thread A affichage pair :  484 
Thread A affichage pair :  486 
Thread A affichage pair :  488 
Thread A affichage pair :  490 
Thread A affichage pair :  492 
Thread A affichage pair :  494 
Thread A affichage pair :  496 
Thread A affichage pair :  498 
Thread A affichage pair :  500 

Thread B affichage impair :  501 
Thread B affichage impair :  503 
Thread B affichage impair :  505 
Thread B affichage impair :  507 
Thread B affichage impair :  509 
Thread B affichage impair :  511 
Thread B affichage impair :  513 
Thread B affichage impair :  515 
Thread B affichage impair :  517 
Thread B affichage impair :  519 

Thread A affichage pair :  502 
Thread A affichage pair :  504 
Thread A affichage pair :  506 
Thread A affichage pair :  508 
Thread A affichage pair :  510 
Thread A affichage pair :  512 
Thread A affichage pair :  514 
Thread A affichage pair :  516 
Thread A affichage pair :  518 
Thread A affichage pair :  520 

Thread B affichage impair :  521 
Thread B affichage impair :  523 
Thread B affichage impair :  525 
Thread B affichage impair :  527 
Thread B affichage impair :  529 
Thread B affichage impair :  531 
Thread B affichage impair :  533 
Thread B affichage impair :  535 
Thread B affichage impair :  537 
Thread B affichage impair :  539 

Thread A affichage pair :  522 
Thread A affichage pair :  524 
Thread A affichage pair :  526 
Thread A affichage pair :  528 
Thread A affichage pair :  530 
Thread A affichage pair :  532 
Thread A affichage pair :  534 
Thread A affichage pair :  536 
Thread A affichage pair :  538 
Thread A affichage pair :  540 

Thread B affichage impair :  541 
Thread B affichage impair :  543 
Thread B affichage impair :  545 
Thread B affichage impair :  547 
Thread B affichage impair :  549 
Thread B affichage impair :  551 
Thread B affichage impair :  553 
Thread B affichage impair :  555 
Thread B affichage impair :  557 
Thread B affichage impair :  559 

Thread A affichage pair :  542 
Thread A affichage pair :  544 
Thread A affichage pair :  546 
Thread A affichage pair :  548 
Thread A affichage pair :  550 
Thread A affichage pair :  552 
Thread A affichage pair :  554 
Thread A affichage pair :  556 
Thread A affichage pair :  558 
Thread A affichage pair :  560 

Thread B affichage impair :  561 
Thread B affichage impair :  563 
Thread B affichage impair :  565 
Thread B affichage impair :  567 
Thread B affichage impair :  569 
Thread B affichage impair :  571 
Thread B affichage impair :  573 
Thread B affichage impair :  575 
Thread B affichage impair :  577 
Thread B affichage impair :  579 

Thread A affichage pair :  562 
Thread A affichage pair :  564 
Thread A affichage pair :  566 
Thread A affichage pair :  568 
Thread A affichage pair :  570 
Thread A affichage pair :  572 
Thread A affichage pair :  574 
Thread A affichage pair :  576 
Thread A affichage pair :  578 
Thread A affichage pair :  580 

Thread B affichage impair :  581 
Thread B affichage impair :  583 
Thread B affichage impair :  585 
Thread B affichage impair :  587 
Thread B affichage impair :  589 
Thread B affichage impair :  591 
Thread B affichage impair :  593 
Thread B affichage impair :  595 
Thread B affichage impair :  597 
Thread B affichage impair :  599 

Thread A affichage pair :  582 
Thread A affichage pair :  584 
Thread A affichage pair :  586 
Thread A affichage pair :  588 
Thread A affichage pair :  590 
Thread A affichage pair :  592 
Thread A affichage pair :  594 
Thread A affichage pair :  596 
Thread A affichage pair :  598 
Thread A affichage pair :  600 

Thread B affichage impair :  601 
Thread B affichage impair :  603 
Thread B affichage impair :  605 
Thread B affichage impair :  607 
Thread B affichage impair :  609 
Thread B affichage impair :  611 
Thread B affichage impair :  613 
Thread B affichage impair :  615 
Thread B affichage impair :  617 
Thread B affichage impair :  619 

Thread A affichage pair :  602 
Thread A affichage pair :  604 
Thread A affichage pair :  606 
Thread A affichage pair :  608 
Thread A affichage pair :  610 
Thread A affichage pair :  612 
Thread A affichage pair :  614 
Thread A affichage pair :  616 
Thread A affichage pair :  618 
Thread A affichage pair :  620 

Thread B affichage impair :  621 
Thread B affichage impair :  623 
Thread B affichage impair :  625 
Thread B affichage impair :  627 
Thread B affichage impair :  629 
Thread B affichage impair :  631 
Thread B affichage impair :  633 
Thread B affichage impair :  635 
Thread B affichage impair :  637 
Thread B affichage impair :  639 

Thread A affichage pair :  622 
Thread A affichage pair :  624 
Thread A affichage pair :  626 
Thread A affichage pair :  628 
Thread A affichage pair :  630 
Thread A affichage pair :  632 
Thread A affichage pair :  634 
Thread A affichage pair :  636 
Thread A affichage pair :  638 
Thread A affichage pair :  640 

Thread B affichage impair :  641 
Thread B affichage impair :  643 
Thread B affichage impair :  645 
Thread B affichage impair :  647 
Thread B affichage impair :  649 
Thread B affichage impair :  651 
Thread B affichage impair :  653 
Thread B affichage impair :  655 
Thread B affichage impair :  657 
Thread B affichage impair :  659 

Thread A affichage pair :  642 
Thread A affichage pair :  644 
Thread A affichage pair :  646 
Thread A affichage pair :  648 
Thread A affichage pair :  650 
Thread A affichage pair :  652 
Thread A affichage pair :  654 
Thread A affichage pair :  656 
Thread A affichage pair :  658 
Thread A affichage pair :  660 

Thread B affichage impair :  661 
Thread B affichage impair :  663 
Thread B affichage impair :  665 
Thread B affichage impair :  667 
Thread B affichage impair :  669 
Thread B affichage impair :  671 
Thread B affichage impair :  673 
Thread B affichage impair :  675 
Thread B affichage impair :  677 
Thread B affichage impair :  679 

Thread A affichage pair :  662 
Thread A affichage pair :  664 
Thread A affichage pair :  666 
Thread A affichage pair :  668 
Thread A affichage pair :  670 
Thread A affichage pair :  672 
Thread A affichage pair :  674 
Thread A affichage pair :  676 
Thread A affichage pair :  678 
Thread A affichage pair :  680 

Thread B affichage impair :  681 
Thread B affichage impair :  683 
Thread B affichage impair :  685 
Thread B affichage impair :  687 
Thread B affichage impair :  689 
Thread B affichage impair :  691 
Thread B affichage impair :  693 
Thread B affichage impair :  695 
Thread B affichage impair :  697 
Thread B affichage impair :  699 

Thread A affichage pair :  682 
Thread A affichage pair :  684 
Thread A affichage pair :  686 
Thread A affichage pair :  688 
Thread A affichage pair :  690 
Thread A affichage pair :  692 
Thread A affichage pair :  694 
Thread A affichage pair :  696 
Thread A affichage pair :  698 
Thread A affichage pair :  700 

Thread B affichage impair :  701 
Thread B affichage impair :  703 
Thread B affichage impair :  705 
Thread B affichage impair :  707 
Thread B affichage impair :  709 
Thread B affichage impair :  711 
Thread B affichage impair :  713 
Thread B affichage impair :  715 
Thread B affichage impair :  717 
Thread B affichage impair :  719 

Thread A affichage pair :  702 
Thread A affichage pair :  704 
Thread A affichage pair :  706 
Thread A affichage pair :  708 
Thread A affichage pair :  710 
Thread A affichage pair :  712 
Thread A affichage pair :  714 
Thread A affichage pair :  716 
Thread A affichage pair :  718 
Thread A affichage pair :  720 

Thread B affichage impair :  721 
Thread B affichage impair :  723 
Thread B affichage impair :  725 
Thread B affichage impair :  727 
Thread B affichage impair :  729 
Thread B affichage impair :  731 
Thread B affichage impair :  733 
Thread B affichage impair :  735 
Thread B affichage impair :  737 
Thread B affichage impair :  739 

Thread A affichage pair :  722 
Thread A affichage pair :  724 
Thread A affichage pair :  726 
Thread A affichage pair :  728 
Thread A affichage pair :  730 
Thread A affichage pair :  732 
Thread A affichage pair :  734 
Thread A affichage pair :  736 
Thread A affichage pair :  738 
Thread A affichage pair :  740 

Thread B affichage impair :  741 
Thread B affichage impair :  743 
Thread B affichage impair :  745 
Thread B affichage impair :  747 
Thread B affichage impair :  749 
Thread B affichage impair :  751 
Thread B affichage impair :  753 
Thread B affichage impair :  755 
Thread B affichage impair :  757 
Thread B affichage impair :  759 

Thread A affichage pair :  742 
Thread A affichage pair :  744 
Thread A affichage pair :  746 
Thread A affichage pair :  748 
Thread A affichage pair :  750 
Thread A affichage pair :  752 
Thread A affichage pair :  754 
Thread A affichage pair :  756 
Thread A affichage pair :  758 
Thread A affichage pair :  760 

Thread B affichage impair :  761 
Thread B affichage impair :  763 
Thread B affichage impair :  765 
Thread B affichage impair :  767 
Thread B affichage impair :  769 
Thread B affichage impair :  771 
Thread B affichage impair :  773 
Thread B affichage impair :  775 
Thread B affichage impair :  777 
Thread B affichage impair :  779 

Thread A affichage pair :  762 
Thread A affichage pair :  764 
Thread A affichage pair :  766 
Thread A affichage pair :  768 
Thread A affichage pair :  770 
Thread A affichage pair :  772 
Thread A affichage pair :  774 
Thread A affichage pair :  776 
Thread A affichage pair :  778 
Thread A affichage pair :  780 

Thread B affichage impair :  781 
Thread B affichage impair :  783 
Thread B affichage impair :  785 
Thread B affichage impair :  787 
Thread B affichage impair :  789 
Thread B affichage impair :  791 
Thread B affichage impair :  793 
Thread B affichage impair :  795 
Thread B affichage impair :  797 
Thread B affichage impair :  799 

Thread A affichage pair :  782 
Thread A affichage pair :  784 
Thread A affichage pair :  786 
Thread A affichage pair :  788 
Thread A affichage pair :  790 
Thread A affichage pair :  792 
Thread A affichage pair :  794 
Thread A affichage pair :  796 
Thread A affichage pair :  798 
Thread A affichage pair :  800 

Thread B affichage impair :  801 
Thread B affichage impair :  803 
Thread B affichage impair :  805 
Thread B affichage impair :  807 
Thread B affichage impair :  809 
Thread B affichage impair :  811 
Thread B affichage impair :  813 
Thread B affichage impair :  815 
Thread B affichage impair :  817 
Thread B affichage impair :  819 

Thread A affichage pair :  802 
Thread A affichage pair :  804 
Thread A affichage pair :  806 
Thread A affichage pair :  808 
Thread A affichage pair :  810 
Thread A affichage pair :  812 
Thread A affichage pair :  814 
Thread A affichage pair :  816 
Thread A affichage pair :  818 
Thread A affichage pair :  820 

Thread B affichage impair :  821 
Thread B affichage impair :  823 
Thread B affichage impair :  825 
Thread B affichage impair :  827 
Thread B affichage impair :  829 
Thread B affichage impair :  831 
Thread B affichage impair :  833 
Thread B affichage impair :  835 
Thread B affichage impair :  837 
Thread B affichage impair :  839 

Thread A affichage pair :  822 
Thread A affichage pair :  824 
Thread A affichage pair :  826 
Thread A affichage pair :  828 
Thread A affichage pair :  830 
Thread A affichage pair :  832 
Thread A affichage pair :  834 
Thread A affichage pair :  836 
Thread A affichage pair :  838 
Thread A affichage pair :  840 

Thread B affichage impair :  841 
Thread B affichage impair :  843 
Thread B affichage impair :  845 
Thread B affichage impair :  847 
Thread B affichage impair :  849 
Thread B affichage impair :  851 
Thread B affichage impair :  853 
Thread B affichage impair :  855 
Thread B affichage impair :  857 
Thread B affichage impair :  859 

Thread A affichage pair :  842 
Thread A affichage pair :  844 
Thread A affichage pair :  846 
Thread A affichage pair :  848 
Thread A affichage pair :  850 
Thread A affichage pair :  852 
Thread A affichage pair :  854 
Thread A affichage pair :  856 
Thread A affichage pair :  858 
Thread A affichage pair :  860 

Thread B affichage impair :  861 
Thread B affichage impair :  863 
Thread B affichage impair :  865 
Thread B affichage impair :  867 
Thread B affichage impair :  869 
Thread B affichage impair :  871 
Thread B affichage impair :  873 
Thread B affichage impair :  875 
Thread B affichage impair :  877 
Thread B affichage impair :  879 

Thread A affichage pair :  862 
Thread A affichage pair :  864 
Thread A affichage pair :  866 
Thread A affichage pair :  868 
Thread A affichage pair :  870 
Thread A affichage pair :  872 
Thread A affichage pair :  874 
Thread A affichage pair :  876 
Thread A affichage pair :  878 
Thread A affichage pair :  880 

Thread B affichage impair :  881 
Thread B affichage impair :  883 
Thread B affichage impair :  885 
Thread B affichage impair :  887 
Thread B affichage impair :  889 
Thread B affichage impair :  891 
Thread B affichage impair :  893 
Thread B affichage impair :  895 
Thread B affichage impair :  897 
Thread B affichage impair :  899 

Thread A affichage pair :  882 
Thread A affichage pair :  884 
Thread A affichage pair :  886 
Thread A affichage pair :  888 
Thread A affichage pair :  890 
Thread A affichage pair :  892 
Thread A affichage pair :  894 
Thread A affichage pair :  896 
Thread A affichage pair :  898 
Thread A affichage pair :  900 

Thread B affichage impair :  901 
Thread B affichage impair :  903 
Thread B affichage impair :  905 
Thread B affichage impair :  907 
Thread B affichage impair :  909 
Thread B affichage impair :  911 
Thread B affichage impair :  913 
Thread B affichage impair :  915 
Thread B affichage impair :  917 
Thread B affichage impair :  919 

Thread A affichage pair :  902 
Thread A affichage pair :  904 
Thread A affichage pair :  906 
Thread A affichage pair :  908 
Thread A affichage pair :  910 
Thread A affichage pair :  912 
Thread A affichage pair :  914 
Thread A affichage pair :  916 
Thread A affichage pair :  918 
Thread A affichage pair :  920 

Thread B affichage impair :  921 
Thread B affichage impair :  923 
Thread B affichage impair :  925 
Thread B affichage impair :  927 
Thread B affichage impair :  929 
Thread B affichage impair :  931 
Thread B affichage impair :  933 
Thread B affichage impair :  935 
Thread B affichage impair :  937 
Thread B affichage impair :  939 

Thread A affichage pair :  922 
Thread A affichage pair :  924 
Thread A affichage pair :  926 
Thread A affichage pair :  928 
Thread A affichage pair :  930 
Thread A affichage pair :  932 
Thread A affichage pair :  934 
Thread A affichage pair :  936 
Thread A affichage pair :  938 
Thread A affichage pair :  940 

Thread B affichage impair :  941 
Thread B affichage impair :  943 
Thread B affichage impair :  945 
Thread B affichage impair :  947 
Thread B affichage impair :  949 
Thread B affichage impair :  951 
Thread B affichage impair :  953 
Thread B affichage impair :  955 
Thread B affichage impair :  957 
Thread B affichage impair :  959 

Thread A affichage pair :  942 
Thread A affichage pair :  944 
Thread A affichage pair :  946 
Thread A affichage pair :  948 
Thread A affichage pair :  950 
Thread A affichage pair :  952 
Thread A affichage pair :  954 
Thread A affichage pair :  956 
Thread A affichage pair :  958 
Thread A affichage pair :  960 

Thread B affichage impair :  961 
Thread B affichage impair :  963 
Thread B affichage impair :  965 
Thread B affichage impair :  967 
Thread B affichage impair :  969 
Thread B affichage impair :  971 
Thread B affichage impair :  973 
Thread B affichage impair :  975 
Thread B affichage impair :  977 
Thread B affichage impair :  979 

Thread A affichage pair :  962 
Thread A affichage pair :  964 
Thread A affichage pair :  966 
Thread A affichage pair :  968 
Thread A affichage pair :  970 
Thread A affichage pair :  972 
Thread A affichage pair :  974 
Thread A affichage pair :  976 
Thread A affichage pair :  978 
Thread A affichage pair :  980 

Thread B affichage impair :  981 
Thread B affichage impair :  983 
Thread B affichage impair :  985 
Thread B affichage impair :  987 
Thread B affichage impair :  989 
Thread B affichage impair :  991 
Thread B affichage impair :  993 
Thread B affichage impair :  995 
Thread B affichage impair :  997 
Thread B affichage impair :  999 

Thread A affichage pair :  982 
Thread A affichage pair :  984 
Thread A affichage pair :  986 
Thread A affichage pair :  988 
Thread A affichage pair :  990 
Thread A affichage pair :  992 
Thread A affichage pair :  994 
Thread A affichage pair :  996 
Thread A affichage pair :  998 
Thread A affichage pair :  1000 
*/
