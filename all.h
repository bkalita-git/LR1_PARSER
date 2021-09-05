#include<stdio.h>
#include<malloc.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
static char ****all_lk;

static char ***tr;
static char ***tr_follow;
static char ***gp_sym;

static int   **gp_tr;
static int   **gp_on_sym;
static int    *tr_point;
static int     use_gp;
static int     new_gpno;



char **make_2d_char_array();
void display_dp_array(char **tmp);
char **make_2d_from_1d_char_array(char *tr);
void display_3d_char_array(char ***text);
char ***send_word_to_3d_char_array(char *word,char ***text);
char ***make_new_line_in_3d_array(char ***text);
char ***make_3d_char_array();
char *make_1d_char_array();
char ***input_to_3d_char_array_from_file(char *file);
char ***input_to_3d_char_array_from_keyboard(); // end with a "$" word
char ***replace_line_in_a_loc_in_3d_char_array(char **line,char***text,int loc); //line should be ended with a "$" word
int check_if_a_word_is_in_3d_array(char ***text,char *word);
int check_if_a_word_is_bold(char *word);
char ***unique_word_from_3d_array(char ***text);
int check_if_a_word_is_of_char(char *word);
int how_many_words_in_3d_array(char ***array);
int check_if_a_word_is_in_2d_array(char **text,char *word);
int terminal(char *word);
char ***send_word_in_a_loc_to_3d_char_array(char *word,int loc,char ***text);
char ***append_unique_word_in_3d_array(char ***first,char *loc,char **line);
char **FIRST(char ***text,int loc,char ***first );
char ***calc_first(char ***text);
char ***calc_follow(char ***text,char ***first);
char ***find_first(char *word,char ***first,char ***lk,char **expr,int loc,int lk_loc);
char ***send_line_to_3d_char_array(char **line,char ***text,int loc);
int **add_num_in_2d_int_array(int n,int **num,int loc);
int if_the_expr_with_lk_already_in_gpno(char **expr,char **lk_line,int point,int gpno);
int expr_with_gpno(char **expr,int gpno, char ***lk,int loc);
int find_last_index_of_3d_char_array(char ***array);
int find_loc_of_a_word_in_1st_index_in_3d_array(char *word,char ***lk);
int is_any_group_on_a_sym(int gpno,char *sym);
void display_2d_int_array(int **arr);
void display_2d_char_array(char **arr);
int **make_new_line_in_2d_int_array(int **array);
int last_actual_index_of_4d_char_array(char ****arr);
int *add_num_in_1d_int_array(int  n,int *num);
int check_if_two_lines_are_similar(char **expr,char **line);
int last_actual_index_of_1d_int_array(int *arr);
int if_the_expr_already_in_gpno(char **expr,int gpno,int point);
void closure(int gpno,char ***first,char ***lk,char ***text);
int if_the_expr_with_lk_already_in_any_gpno(char **expr,char **lk_line,int point);
void move_expr(int gpno,char **line,int point,char ***first,char ***lk,char ***text);
void move_all_tr(int gpno,char ***first,char ***lk,char ***text);
char ***clear_words_in_a_loc_from_a_pos_from_3d_char_array(int loc,int pos,char ***arr);
/*******************************************************************************************/
/*******************************************************************************************/
char ***p_table_attr(char ***text) {
	char ***temp = make_3d_char_array();
	int g = 0;
	while (strlen(text[g][0])>0) {	
		int w  = 0;
		while (strcmp(text[g][w],"$")!=0) {
			if (check_if_a_word_is_in_2d_array(temp[0],text[g][w])==-1 && strcmp(text[g][w],":")!=0)
				temp = send_word_in_a_loc_to_3d_char_array(text[g][w],0,temp);
			w++;
		}
		w = 0;
		g++;
	}
	return temp;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***append_unique_word_in_3d_array_by_int_loc(char ***first,int loc,char **line){
	int i = loc;
	int j = 0;
	while (strcmp(line[j],"$")!=0) {
		if (check_if_a_word_is_in_2d_array(first[i],line[j])==-1 && terminal(line[j]))
			first = send_word_in_a_loc_to_3d_char_array(line[j],i,first);
		j++;
	}
	return first;
}
/*******************************************************************************************/
/*******************************************************************************************/
void move(int gpno,char **line,char **tr_lk,int point,char ***first,char ***lk,char ***text) {
	int new_gp = is_any_group_on_a_sym(gpno,line[point]);
	if(new_gp!=-1) { 
		if (!if_the_expr_with_lk_already_in_gpno(line,tr_lk,point+1,new_gp) ) {
			tr = send_line_to_3d_char_array(line,tr,find_last_index_of_3d_char_array(tr));
			tr = make_new_line_in_3d_array(tr);
			tr_point = add_num_in_1d_int_array(point+1,tr_point);
			tr_follow = send_line_to_3d_char_array(tr_lk,tr_follow,find_last_index_of_3d_char_array(tr_follow));
			tr_follow = make_new_line_in_3d_array(tr_follow);
			gp_tr = add_num_in_2d_int_array(find_last_index_of_3d_char_array(tr)-1,gp_tr,new_gp);
			if (strcmp(line[point+1],"$")!=0)
				closure(new_gp,first,lk,text);
		}
		else
			return;
	}
	else if ((new_gp = if_the_expr_with_lk_already_in_any_gpno(line,tr_lk,point+1))!=-1) {
			gp_sym = send_word_in_a_loc_to_3d_char_array(line[point],gpno,gp_sym);
			gp_on_sym = add_num_in_2d_int_array(new_gp,gp_on_sym,gpno);
	}
	else if (strcmp(line[point],"$")!=0){
			new_gp = ++use_gp;
			gp_tr = make_new_line_in_2d_int_array(gp_tr);
			gp_sym = make_new_line_in_3d_array(gp_sym);
			gp_on_sym = make_new_line_in_2d_int_array(gp_on_sym);
			tr = send_line_to_3d_char_array(line,tr,find_last_index_of_3d_char_array(tr));
			tr = make_new_line_in_3d_array(tr);
			tr_point = add_num_in_1d_int_array(point+1,tr_point);
			tr_follow = send_line_to_3d_char_array(tr_lk,tr_follow,find_last_index_of_3d_char_array(tr_follow));
			tr_follow = make_new_line_in_3d_array(tr_follow);
			gp_tr = add_num_in_2d_int_array(find_last_index_of_3d_char_array(tr)-1,gp_tr,new_gp);
			gp_sym = send_word_in_a_loc_to_3d_char_array(line[point],gpno,gp_sym);
			gp_on_sym = add_num_in_2d_int_array(new_gp,gp_on_sym,gpno);
			if (strcmp(line[point+1],"$")!=0)
				closure(new_gp,first,lk,text);
	}
}
/*******************************************************************************************/
/*******************************************************************************************/
void move_all_tr(int gpno,char ***first,char ***lk,char ***text) {
	int t = 0;
	while(gp_tr[gpno][t]!=pow(2,13)-1) {
		move(gpno,tr[gp_tr[gpno][t]],tr_follow[gp_tr[gpno][t]],tr_point[gp_tr[gpno][t]],first,lk,text);
		t++;
	}
}
/*******************************************************************************************/
/*******************************************************************************************/
void closure(int gpno,char ***first,char ***lk,char ***text) {
	int i = 0;
	int j = 0;
 	while (gp_tr[gpno][i]!=(pow(2,13)-1)) {
		while (strcmp(text[j][0],"$")!=0 && strlen(text[j][0])>0) {
			if (strcmp(text[j][0],tr[gp_tr[gpno][i]][tr_point[gp_tr[gpno][i]]])==0 ) {
				if(!terminal(tr[gp_tr[gpno][i]][tr_point[gp_tr[gpno][i]]]))
					lk = find_first(tr[gp_tr[gpno][i]][tr_point[gp_tr[gpno][i]]],
					                first,lk,
                                                        tr[gp_tr[gpno][i]],
                                                        tr_point[gp_tr[gpno][i]],
							gp_tr[gpno][i]
                                                       );				
				if(if_the_expr_already_in_gpno(text[j],gpno,2)==-1) {
					tr = send_line_to_3d_char_array(text[j],tr,find_last_index_of_3d_char_array(tr));
					tr = make_new_line_in_3d_array(tr);
					gp_tr = add_num_in_2d_int_array(find_last_index_of_3d_char_array(tr)-1,gp_tr,gpno);
					tr_point = add_num_in_1d_int_array(2,tr_point);
					tr_follow = send_line_to_3d_char_array(
								lk[find_loc_of_a_word_in_1st_index_in_3d_array(text[j][0],lk)],
							        tr_follow,find_last_index_of_3d_char_array(tr_follow)
				                    );
					tr_follow = make_new_line_in_3d_array(tr_follow);
					
				}
				else {
					int tr_pos = if_the_expr_already_in_gpno(text[j],gpno,2);
					tr_follow = append_unique_word_in_3d_array_by_int_loc(
							    tr_follow,tr_pos,
							    lk[find_loc_of_a_word_in_1st_index_in_3d_array(text[j][0],lk)]
						    );
					int vloc = find_loc_of_a_word_in_1st_index_in_3d_array(text[j][0],lk);
					lk = replace_line_in_a_loc_in_3d_char_array(tr_follow[tr_pos],lk,vloc);
				}
			}	
			j++;
		}
		j = 0;
		i++;
	}
	move_all_tr(gpno,first,lk,text);
}
/*******************************************************************************************/
/*******************************************************************************************/
void move_expr(int gpno,char **line,int point,char ***first,char ***lk,char ***text) {
	tr = send_line_to_3d_char_array(line,tr,find_last_index_of_3d_char_array(tr));
	tr = make_new_line_in_3d_array(tr);
	tr_point = add_num_in_1d_int_array(point,tr_point);
	tr_follow = send_line_to_3d_char_array(lk[find_loc_of_a_word_in_1st_index_in_3d_array(line[0],lk)],
                                               tr_follow,find_last_index_of_3d_char_array(tr_follow));
	lk = find_first(line[point],first,lk,line,point,0);
	tr_follow = make_new_line_in_3d_array(tr_follow);
	gp_tr = add_num_in_2d_int_array(find_last_index_of_3d_char_array(tr)-1,gp_tr,gpno);
	closure(gpno,first,lk,text);
}
/*******************************************************************************************/
/*******************************************************************************************/
char ****make_new_line_in_4d_char_array(char ****all_lk) {
	all_lk	= (char ****)realloc(all_lk,sizeof *all_lk * (last_actual_index_of_4d_char_array(all_lk)+2));
	return all_lk;
}
/*******************************************************************************************/
/*******************************************************************************************/
int last_actual_index_of_4d_char_array(char ****arr) {
	int i = 0;
	while (arr[i]!=0)
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***duplicate_3d_char_array(char ***original) {
	char ***newar  = make_3d_char_array();
	int i = 0;
	while (strcmp(original[i][0],"$")!=0) {
		newar = replace_line_in_a_loc_in_3d_char_array(original[i],newar,i);
		newar = make_new_line_in_3d_array(newar);
		i++;
	}
	return  newar;
}
/*******************************************************************************************/
/*******************************************************************************************/
int loc_of_a_word_in_2d_char_array(char *word,char **arr) {
	int i = 0;
	while (strcmp(arr[i],word)!=0)
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
void display_2d_char_array(char **arr) {
	int i = 0;
	while (strcmp(arr[i],"$")!=0) {
		printf("%s ",arr[i]);
		i++;
	}
	printf("\t");
}
/*******************************************************************************************/
/*******************************************************************************************/
int last_actual_index_of_2d_char_array(char **arr) {
	int i = 0;
	while (strcmp(arr[i],"$")!=0)
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
int last_actual_index_of_1d_int_array(int *arr) {
	int i = 0;
	while (arr[i]!=(pow(2,13)-1))
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
int last_actual_index_of_2d_int_array(int **arr) {
	int i = 0;
	while (arr[i][0]!=(pow(2,14)-1))
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
int last_actual_index_of_3d_char_array(char ***arr) {
	int i = 0;
	while (strcmp(arr[i][0],"$")!=0)
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
void display_actual_3d_char_array(char ***arr) {
	int i = 0;
	int j = 0;
	while (strcmp(arr[i][0],"$")!=0) {
		while (strcmp(arr[i][j],"$")!=0) {
			printf ("%s ",arr[i][j]);
			j++;
		}
		j = 0;
		i++;
		printf ("\n");
	}
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***clear_words_in_a_loc_from_a_pos_from_3d_char_array(int loc,int pos,char ***arr) {
	while (strcmp(arr[loc][pos],"$")!=0) {
		strcpy(arr[loc][pos],"$");
		pos++;
	}
	return arr;
}
/*******************************************************************************************/
/*******************************************************************************************/
		   //find_first(text[j][2],first,lk,text[j],2);
char ***find_first(char *word,char ***first,char ***lk,char **expr,int loc,int lk_loc) {
	int f_word_pos = find_loc_of_a_word_in_1st_index_in_3d_array(expr[0],lk);
	int word_pos = find_loc_of_a_word_in_1st_index_in_3d_array(word,lk);
	char ***temp = make_3d_char_array();
	temp = send_line_to_3d_char_array(lk[f_word_pos],temp,0);
	if (strcmp(expr[loc+1],"$")==0) {
		lk = clear_words_in_a_loc_from_a_pos_from_3d_char_array(word_pos,2,lk);
		int k = 2;
		while (strcmp(tr_follow[lk_loc][k],"$")!=0) {
			lk = send_word_in_a_loc_to_3d_char_array(tr_follow[lk_loc][k],word_pos,lk); 
			k++;
		}
	}
	else if (terminal(expr[loc+1])) {
		lk = clear_words_in_a_loc_from_a_pos_from_3d_char_array(word_pos,2,lk);
		lk = send_word_in_a_loc_to_3d_char_array(expr[loc+1],word_pos,lk); 
		
	}
	else  {
		lk = clear_words_in_a_loc_from_a_pos_from_3d_char_array(word_pos,2,lk);
		int k = 2;
		while (strcmp(first[find_loc_of_a_word_in_1st_index_in_3d_array(expr[loc+1],first)][k],"$")!=0) {
			lk = send_word_in_a_loc_to_3d_char_array(first[find_loc_of_a_word_in_1st_index_in_3d_array(expr[loc+1],first)][k],
								 word_pos,lk); 
			k++;
		}	
	}	
	return lk;
}
/*******************************************************************************************/
/*******************************************************************************************/
void display_2d_int_array(int **arr) {
	int i = 0;
	int j = 0;
	while (arr[i][0]!=(pow(2,13)-1)) {
		while (arr[i][j]!=(pow(2,13)-1)) {
			printf ("%d ",arr[i][j]);
			j ++;
		}
		if (arr[i+1][0]==(pow(2,14)-1))
			break;
		printf ("\n");
		j = 0;
		i++;
	}	
}
/*******************************************************************************************/
/*******************************************************************************************/
int find_last_index_of_3d_char_array(char ***array) {
	int i = 0;
	while (strlen(array[i][0])>0) {
			if (strcmp(array[i][0],"$")==0)
				break;
		i++;
	}
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
int find_loc_of_a_word_in_1st_index_in_3d_array(char *word,char ***lk) {
	int i = 0;
	while (strcmp(lk[i][0],word)!=0)
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
int check_if_two_lines_are_similar(char **expr,char **line) {
	int i = 0;
	int j = 0;
	while (strcmp(expr[i],"$")!=0) 
		i++;
	while (strcmp(line[j],"$")!=0)
		j++;
	i = (i<j)?j:i;
	j = 0;
	while (j<i) {
		if (strcmp(expr[j],line[j])!=0)
			return 0;
		j++;
	}
	
	return 1;
}
/*******************************************************************************************/
/*******************************************************************************************/
int expr_with_gpno(char **expr,int gpno, char ***lk,int loc) {
	int t = 0;
	int x = 0;
	while (gp_tr[gpno][t]!=(pow(2,13)-1)) {
		if(check_if_two_lines_are_similar(expr,tr[gp_tr[gpno][t]]) && 
		   check_if_two_lines_are_similar(lk[find_loc_of_a_word_in_1st_index_in_3d_array(expr[0],lk)],tr_follow[gp_tr[gpno][t]]) &&
		   loc != tr_point[gp_tr[gpno][t]])
			x = 1;
		t++;
	}
        t = 0;
	while (gp_tr[gpno][t]!=(pow(2,13)-1)) {
		if(check_if_two_lines_are_similar(expr,tr[gp_tr[gpno][t]]) && 
		   check_if_two_lines_are_similar(lk[find_loc_of_a_word_in_1st_index_in_3d_array(expr[0],lk)],tr_follow[gp_tr[gpno][t]]) &&
		   loc == tr_point[gp_tr[gpno][t]])
			x = 2;
		t++;
	}
	return x;
}
/*******************************************************************************************/
/*******************************************************************************************/
int is_any_group_on_a_sym(int gpno,char *sym) {
	int i = 0;
	while (strcmp(gp_sym[gpno][i],"$")!=0) {
		if (strcmp(gp_sym[gpno][i],sym)==0)
			return gp_on_sym[gpno][i];
		i++;
	}
	return -1;
}
/*******************************************************************************************/
/*******************************************************************************************/
int find_last_index_of_1d_int_array(int *array) {
	int i = 0;
	while (array[i]!=(pow(2,13)-1))
		i++;
	return i;
}
/*******************************************************************************************/
/*******************************************************************************************/
int if_the_expr_already_in_gpno(char **expr,int gpno,int point) {
	int t = 0;
	while (gp_tr[gpno][t]!=(pow(2,13)-1)) {
		if(check_if_two_lines_are_similar(expr,tr[gp_tr[gpno][t]]) && point == tr_point[gp_tr[gpno][t]])
			return gp_tr[gpno][t];
		t++;
	}
	return -1;
}
/*******************************************************************************************/
/*******************************************************************************************/
int if_the_expr_with_lk_already_in_any_gpno(char **expr,char **lk_line,int point) {
	int i = 0;
	int t = 0;
	while (gp_tr[i][0]!=(pow(2,14)-1)) {
		while (gp_tr[i][t]!=(pow(2,13)-1)) {
			if(check_if_two_lines_are_similar(expr,tr[gp_tr[i][t]]) && 
		  	 check_if_two_lines_are_similar(lk_line,tr_follow[gp_tr[i][t]]) &&
		  	 tr_point[gp_tr[i][t]] == point
		 	 )
				return i;
			t++;
		}
		t = 0;
		i++;
	}
	return -1;
}
/*******************************************************************************************/
/*******************************************************************************************/
int if_the_expr_with_lk_already_in_gpno(char **expr,char **lk_line,int point,int gpno) {
	int t = 0;
	while (gp_tr[gpno][t]!=(pow(2,13)-1)) {
		if(check_if_two_lines_are_similar(expr,tr[gp_tr[gpno][t]]) && 
		   check_if_two_lines_are_similar(lk_line,tr_follow[gp_tr[gpno][t]]) &&
		   tr_point[gp_tr[gpno][t]] == point
		  )
			return 1;
		t++;
	}
	return 0;
}
/*******************************************************************************************/
/*******************************************************************************************/
char **FOLLOW(char ***text,int i,int j,char ***follow,char ***first) {
	char ***ter = make_3d_char_array();
	int flag = 0;
	if (strcmp(text[i][j+1],":")!=0 && strcmp(text[i][j],":")!=0 ) {
		if (terminal(text[i][j+1]) && strcmp(text[i][j+1],"$")!=0) {
			ter = send_word_to_3d_char_array(text[i][j+1],ter);
			return ter[0];
		}
		else  {
			int k = 0;
			while (strlen(first[k][0])>0 && strcmp(first[k][0],text[i][j+1])!=0)
				k++;	
			int l = 0;
			while ( strcmp(first[k][l],"$")!=0 ) {
				if (terminal(first[k][l]) )
					if (strcmp(first[k][l],"NULL")==0)
						flag = 1;
					else 
						ter = send_word_to_3d_char_array(first[k][l],ter);
				l++;
			}
			if (flag == 1 || strcmp(text[i][j+1],"$")==0) {
				int k = 0;
				while (strlen(follow[k][0])>0 && strcmp(follow[k][0],text[i][0])!=0)
					k++;
				int l = 0;
				while ( strcmp(follow[k][l],"$")!=0) {
					if (terminal(follow[k][l]) && strcmp(follow[k][l],"NULL")!=0)
						ter = send_word_to_3d_char_array(follow[k][l],ter);
					l++;
				}
			}
		}
			
	}

	return ter[0];
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***calc_follow(char ***text,char ***first){
	char ***follow = unique_word_from_3d_array(text);
	if (strlen(follow[0][0])>0)
		follow = send_word_in_a_loc_to_3d_char_array("START",0,follow); 
	int ne = 1;
	int old = 0;
	while (ne!=old) {
		ne = 0;
		ne = how_many_words_in_3d_array(follow);
		int i = 0;
		int j = 0;
		while (strlen(text[i][0])>0) {
			while (strcmp(text[i][j],"$")!=0) {
				if (!terminal(text[i][j])) {
					follow = append_unique_word_in_3d_array(follow,text[i][j],FOLLOW(text,i,j,follow,first));
				}
				j++;
			}
			j = 0;
			i++;
		}
		old = how_many_words_in_3d_array(follow);
	}
	return follow;
}
/*******************************************************************************************/
/*******************************************************************************************/
int terminal(char *word) {
	if( !check_if_a_word_is_of_char(word) || check_if_a_word_is_bold(word) || strcmp(word,"$")==0 )
		return 1;
	return 0;
}
/*******************************************************************************************/
/*******************************************************************************************/
char **FIRST(char ***text,int loc,char ***first ) {
	if (terminal(text[loc][2])) {
		char ***ter = make_3d_char_array();
		if (strcmp(text[loc][2],"$")==0)
			ter = send_word_to_3d_char_array("NULL",ter);	
		else 
			ter = send_word_to_3d_char_array(text[loc][2],ter);
		return ter[0];
	}
	else{
		int i = 0;
		while (strlen(first[i][0])>0 && strcmp(first[i][0],text[loc][2])!=0)
			i++;
		return first[i];
	}
		
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***calc_first(char ***text) {
	char ***first = unique_word_from_3d_array(text);
	int ne = 1;
	int old = 0;
	while (ne!=old) {
		ne = 0;
		ne = how_many_words_in_3d_array(first);
		int i = 0;
		while (strlen(text[i][0])>0) {
			first = append_unique_word_in_3d_array(first,text[i][0],FIRST(text,i,first));
			i++;
		}
		old = how_many_words_in_3d_array(first);
	}
	return first;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***send_word_in_a_loc_to_3d_char_array(char *word,int loc,char ***text) {
	int tj = 0;
	int size = strlen(word);
	while (strcmp(text[loc][tj],"$")!=0)
		tj++;
	if (text[loc][0][0]=='\0')	
		tj--;
	text[loc][tj] = (char *)malloc(sizeof *text[loc][tj] * (size+1));
	strcpy(text[loc][tj],word);
	text[loc]     = (char**)realloc(text[loc],sizeof *text[loc] * (++tj+1));
	text[loc][tj] = (char *)malloc(sizeof *text[loc][tj] * 3);
	strcpy(text[loc][tj],"$");
	return text;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***append_unique_word_in_3d_array(char ***first,char *loc,char **line){
	int i = 0;
	while (strlen(first[i][0])>0 && strcmp(first[i][0],loc)!=0)
		i++;
	int j = 0;
	while (strcmp(line[j],"$")!=0) {
		if (check_if_a_word_is_in_2d_array(first[i],line[j])==-1 && terminal(line[j]))
			first = send_word_in_a_loc_to_3d_char_array(line[j],i,first);
		j++;
	}
	return first;
}
/*******************************************************************************************/
/*******************************************************************************************/
int how_many_words_in_3d_array(char ***array) {
	int i = 0;
	int s = 0;
	int j =0;
	while (strlen(array[i][0])>0) {
		while (strcmp(array[i][j],"$")!=0) {
			s++;
			j++;
		}
		j = 0;
		i++;
	}
	return s;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***unique_word_from_3d_array(char ***text) {
	char ***word = make_3d_char_array();
	int i = 0;
	int j = 0;
	while (strlen(text[i][0])>0) {
		while(strcmp(text[i][j],"$")!=0) {
			if ( !check_if_a_word_is_in_3d_array(word,text[i][j]) && 
			     check_if_a_word_is_of_char(text[i][j]) && 
			     !check_if_a_word_is_bold(text[i][j]) ) {
				word = send_word_to_3d_char_array(text[i][j],word);
				word = send_word_to_3d_char_array(":",word);
				word = make_new_line_in_3d_array(word);
				
			}
			j++;
		}
		j = 0;
		i++;
	}
	return word;
}
/*******************************************************************************************/
/*******************************************************************************************/
int check_if_a_word_is_of_char(char *word) {
	int i = 0;
	while (word[i]!='\0' && isalpha(word[i]))
		i++;
	if (word[i]=='\0')
		return 1;
	else 
		return 0;
}
/*******************************************************************************************/
/*******************************************************************************************/
int check_if_a_word_is_bold(char *word) {
	int i = 0;
	while (word[i]!='\0' && isupper(word[i]))
		i++;
	if (word[i]=='\0')
		return 1;
	else 
		return 0;
}
/*******************************************************************************************/
/*******************************************************************************************/
int check_if_a_word_is_in_3d_array(char ***text,char *word) {
	int i = 0;
	int j = 0;
	while (strlen(text[i][0])>0) {
		while (strcmp(text[i][j],"$")!=0 && strcmp(text[i][j],word)!=0)
			j++;
		if (strcmp(text[i][j],word)==0) {
			return 1; 
			break;
		}
		j = 0;
		i++;	
	}	
	return 0;
}
/*******************************************************************************************/
/*******************************************************************************************/
int check_if_a_word_is_in_2d_array(char **text,char *word) {
	int j = 0;
		while (strcmp(text[j],"$")!=0 && strcmp(text[j],word)!=0)
			j++;
		if (strcmp(text[j],word)==0)
			return j; 	
	return -1;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***replace_line_in_a_loc_in_3d_char_array(char **line,char***text,int loc) {
	if (strcmp(line[0],"$")!=0) {
	text[loc] = (char **)malloc(sizeof *text[loc] * 1);
	text[loc][0] = (char *)malloc(sizeof *text[loc][0] * 3);
	strcpy(text[loc][0],"$");
	}
	int i = 0;
	int tj = 0;
	while (strcmp(line[i],"$")!=0) {
		int size = strlen(line[i]);
		text[loc][tj] = (char *)malloc(sizeof *text[loc][tj] * (size+1));
		strcpy(text[loc][tj],line[i]);
		text[loc]     = (char**)realloc(text[loc],sizeof *text[loc] * (++tj+1));
		text[loc][tj] = (char *)malloc(sizeof *text[loc][tj] * 3);
		strcpy(text[loc][tj],"$");
		i++;
		
	}
	return text;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***send_line_to_3d_char_array(char **line,char ***text,int loc) {
	text = make_new_line_in_3d_array(text);
	int i = 0;
	while (strcmp(text[i][0],"$")!=0)
		i++;
	i=i-2;
	while (i!=loc) {
		text = replace_line_in_a_loc_in_3d_char_array(text[i],text,i+1);
		i--;	
	}
	if (i==loc)
		text = replace_line_in_a_loc_in_3d_char_array(text[i],text,i+1);
		text = replace_line_in_a_loc_in_3d_char_array(line,text,loc);
	return text;
}
/*******************************************************************************************/
/*******************************************************************************************/
char *make_1d_char_array() {
	char *temp = (char *)malloc(sizeof *temp * 1);
	temp[0] = '\0';
	return temp;
}
/*******************************************************************************************/
/*******************************************************************************************/
int *make_1d_int_array() {
	int *temp = (int *)malloc(sizeof *temp * 1);
	temp[0] = pow(2,13)-1;
	return temp;
}
/*******************************************************************************************/
/*******************************************************************************************/
int *add_num_in_1d_int_array(int  n,int *num) {
	int i = 0;
	while (num[i]!=(pow(2,13)-1))
		i++;
	num = (int *)realloc(num,sizeof *num * (i+2));
	num[i] = n;
	num[i+1] = (pow(2,13)-1);
	return num;
}
/*******************************************************************************************/
/*******************************************************************************************/
int check_if_a_num_in_1d_int_array(int n, int *num) {
	int i = 0;
	while (num[i]!=(pow(2,13)-1)) {
		if (num[i]==n)
			return 1;
	
		i++;
	}
	return 0;
}
/*******************************************************************************************/
/*******************************************************************************************/
char **make_2d_char_array() {
	int row = 2,
	    col = 1;
	char 	  **tr = (char **)malloc(sizeof *tr * row);
	     tr[row-2] = (char  *)malloc(sizeof *tr[row-2] * col  );
	     tr[row-1] = (char  *)malloc(sizeof *tr[row-1] * col+2);
	     tr[row-2][0] = '\0';
	     strcpy(tr[row-1],"\0");
	return tr;
}
/*******************************************************************************************/
/*******************************************************************************************/
int **make_2d_int_array() {
	int row = 2,
	    col = 1;
	int 	  **tr = (int **)malloc(sizeof *tr * row);
	     tr[row-2] = (int  *)malloc(sizeof *tr[row-2] * col  );
	     tr[row-1] = (int  *)malloc(sizeof *tr[row-1] * col  );
	     tr[row-2][0] = (pow(2,13)-1);
	     tr[row-1][0] = (pow(2,14)-1);
	return tr;
}
/*******************************************************************************************/
/*******************************************************************************************/
int **add_num_in_2d_int_array(int n,int **num,int loc) {
	int i = 0;
	while (num[loc][i]!=(pow(2,13)-1))
		i++ ;
	num[loc] = (int *)realloc(num[loc],sizeof *num[loc] * (i+2));
	num[loc][i] = n;
	num[loc][i+1] = (pow(2,13)-1);
	return num;
}
/*******************************************************************************************/
/*******************************************************************************************/
int **make_new_line_in_2d_int_array(int **num) {
	int i = 0;
	while (num[i][0]!=(pow(2,14)-1))
		i++;
	num = (int **)realloc(num,sizeof *num * (i+2));
	num[i][0] = (pow(2,13)-1);
	num[i+1] = (int *)malloc(sizeof *num[i+1] * 1);
	num[i+1][0] = (pow(2,14)-1);
	return num;
}
/*******************************************************************************************/
/*******************************************************************************************/
char **make_2d_from_1d_char_array(char *tr) {
	char **word = make_2d_char_array();
	int row = 0;
	int col = 0;
	int i   = 0;
	while (tr[i]!='\0') {
		word[row][col] = tr[i];
		word[row]      = (char *)realloc(word[row],sizeof *word[row] * (++col+1));
		word[row][col] = '\0';
		word = (char **)realloc(word,sizeof *word * (++row+1));
		col = 0;
		word[row] = (char *)malloc(sizeof word[row] * (4));
		strcpy(word[row],"\0");
		i++;
	}
	return word;
}
/*******************************************************************************************/
/*******************************************************************************************/
void display_3d_char_array(char ***text) {
	int i = 0;
	int j = 0;
	while (strlen(text[i][0])>0) {
		while(strcmp(text[i][j],"$")!=0) {
			printf("%s ",text[i][j]);
			j++;
		}
		if (strcmp(text[i+1][0],"$")!=0)
			printf("\n");
		else 
			break;
		j = 0;
		i++;
	}
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***send_word_to_3d_char_array(char *word,char ***text) {
	int ti = 0;
	int tj = 0;
	int size = strlen(word);
	while (strcmp(text[ti][0],"$")!=0)
		ti++;
	--ti;
	while (strcmp(text[ti][tj],"$")!=0)
		tj++;
	if (text[ti][0][0]=='\0')	
		tj--;
	text[ti][tj] = (char *)malloc(sizeof *text[ti][tj] * (size+1));
	strcpy(text[ti][tj],word);
	text[ti]     = (char**)realloc(text[ti],sizeof *text[ti] * (++tj+1));
	text[ti][tj] = (char *)malloc(sizeof *text[ti][tj] * 3);
	strcpy(text[ti][tj],"$");
	return text;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***make_new_line_in_3d_array(char ***text) {
	int ti = 0;
	while (strcmp(text[ti][0],"$")!=0)
		ti++;
	ti--;
	text = (char ***)realloc(text,sizeof *text * (++ti+2));
	text[ti] = (char **) malloc(sizeof *text[ti] * 2);
	text[ti+1] = (char **) malloc(sizeof *text[ti+1] * 1);
	text[ti][0] = (char *)malloc(sizeof *text[ti][0] * 1);
	text[ti][0][0] = '\0';
	text[ti][1] = (char *)malloc(sizeof *text[ti][1] * 3);
	strcpy(text[ti][1],"$");
	text[ti+1][0] = (char *)malloc(sizeof *text[ti+1][0] * 3);
	strcpy(text[ti+1][0],"$");
	return text;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***make_3d_char_array() {
	char ***text = (char ***)malloc(sizeof *text * 2);
	text[0] = (char **) malloc(sizeof *text[0] * 2);
	text[1] = (char **) malloc(sizeof *text[1] * 1);
	text[0][0] = (char *)malloc(sizeof *text[0][0] * 1);
	text[0][0][0] = '\0';
	text[0][1] = (char *)malloc(sizeof *text[0][1] * 3);
	strcpy(text[0][1],"$");
	text[1][0] = (char *)malloc(sizeof *text[1][0] * 3);
	strcpy(text[1][0],"$");
	return text;
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***input_to_3d_char_array_from_file(char *file) {
	char ***text = make_3d_char_array();
	FILE *fp;
	fp = fopen(file,"r");
	char *temp = make_1d_char_array();
	char ch;
	int i = 0;
	while((ch=fgetc(fp))!=EOF) {
		if (ch!=' ' && ch!='\n' && ch!='\t') {
			temp[i] = ch;
			temp = (char *)realloc(temp,sizeof *temp * (++i+1));
			temp[i] = '\0';
		}
		if (ch==' ' || ch=='\t') {
			i = 0;
			if (strlen(temp)>0) {
				text = send_word_to_3d_char_array(temp,text);
				temp[0]='\0';
				
			}
		}
		if (ch == '\n') {
			i = 0;
			if (strlen(temp)>0) {
				text = send_word_to_3d_char_array(temp,text);
				temp[0]='\0';
				text = make_new_line_in_3d_array(text);
			}
				
		}
	}
	text = make_new_line_in_3d_array(text);
	fclose(fp);
	return text;	
}
/*******************************************************************************************/
/*******************************************************************************************/
char ***input_to_3d_char_array_from_keyboard() {
	char ***text = make_3d_char_array();
	char *temp = make_1d_char_array();
	char ch;
	int i = 0;
	printf("\nenter texts\n");
	while((ch=getchar())!='$') {
		if (ch!=' ' && ch!='\n' && ch!='\t') {
			temp[i] = ch;
			temp = (char *)realloc(temp,sizeof *temp * (++i+1));
			temp[i] = '\0';
		}
		if (ch==' ' || ch=='\t') {
			i = 0;
			if (strlen(temp)>0) {
				text = send_word_to_3d_char_array(temp,text);
				temp[0]='\0';
				
			}
		}
		if (ch == '\n') {
			i = 0;
			if (strlen(temp)>0) {
				text = send_word_to_3d_char_array(temp,text);
				temp[0]='\0';
				text = make_new_line_in_3d_array(text);
			}
		}
	}
	return text;	
}
/*******************************************************************************************/
/****************************************/
void display_dp_array(char **tmp) {
	int i = 0;
	while (strcmp(tmp[i],"\0")!=0) {
		printf("%s\n",tmp[i]);
		i++;
	}
}
/****************************************/

