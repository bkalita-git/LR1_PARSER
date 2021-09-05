#include "all.h"

int p_attr_index(char **p_attr,char *word) {
			return check_if_a_word_is_in_2d_array(p_attr,word);
		}

void main() {
		all_lk    = (char ****)malloc(sizeof *all_lk * 2);
		tr        = make_3d_char_array();
		tr_follow = make_3d_char_array();
		gp_sym = make_3d_char_array(); //remember like display

		gp_tr = make_2d_int_array();
		gp_on_sym = make_2d_int_array();
		
		tr_point = make_1d_int_array();
		
		use_gp = 0;
		char ***text = input_to_3d_char_array_from_file("id");
		char ***first = calc_first(text);
		char ***follow = calc_follow(text,first);
		char ***lk = unique_word_from_3d_array(text);
		lk = send_word_in_a_loc_to_3d_char_array("START",0,lk);
		move_expr(0,text[0],2,first,lk,text);

		
		char ***temp = p_table_attr(text);
		temp = send_word_in_a_loc_to_3d_char_array("START",0,temp);
		char **p_attr = temp[0];
		int col = last_actual_index_of_2d_char_array(p_attr);
		int row = last_actual_index_of_2d_int_array(gp_tr);
		int p_table[row][col];
		int i = 0;
		int j = 0;
		while (i<row){
			while (j<col) {
				p_table[i][j]=-1111;
				j++;
			}
			j = 0;
			i++;
		}
		


		int gpno = 0;
		while (gpno<row){
			int s = 0;
			int trno = 0;
			while (gp_tr[gpno][trno]!=(pow(2,13)-1)) {
				if (last_actual_index_of_2d_char_array(tr[gp_tr[gpno][trno]]) == tr_point[gp_tr[gpno][trno]]) {
					int r_by = 0;
					while (gp_tr[0][r_by]!=pow(2,13)-1) {
						if(check_if_two_lines_are_similar(tr[gp_tr[0][r_by]],tr[gp_tr[gpno][trno]])) {
							break;
						}
						r_by++;	
					}
					int v = 2;
					while (strcmp(tr_follow[gp_tr[gpno][trno]][v],"$")!=0 ) {
						if (r_by == 0) 
							p_table[gpno][p_attr_index(p_attr,tr_follow[gp_tr[gpno][trno]][v])]=1111;
						else 
							p_table[gpno][p_attr_index(p_attr,tr_follow[gp_tr[gpno][trno]][v])]=-r_by;
						v++;
					}
				}
				trno++;
			}
			while (strcmp(gp_sym[gpno][s],"$")!=0) {
				if(strlen(gp_sym[gpno][s])>0)
					p_table[gpno][p_attr_index(p_attr,gp_sym[gpno][s])]=gp_on_sym[gpno][s];
				s++;
			}
			s = 0;
			gpno++;
		}
		/*i = 0;
		printf("group\t");
		while (strcmp(p_attr[i],"$")!=0) {
			printf("%s\t",p_attr[i]);
			i++;
		}
		printf("\n");

		i = 0;
		j = 0;
		printf("\n");
		while (i<row){
			printf("%d\t",i);
			while (j<col) {
				if (p_table[i][j]==-1111)
					printf("\t");
				else
				printf("%d\t",p_table[i][j]);
				j++;
			}
			printf("\n");
			j = 0;
			i++;
		}

	
		int g = 0;
		int t = 0;
		printf("\n");
		while (gp_tr[g][0]!=(pow(2,14)-1)) {
			printf("\n%d\n",g);
			while (gp_tr[g][t]!=(pow(2,13)-1)){
				display_2d_char_array(tr[gp_tr[g][t]]);
				printf("|");
				display_2d_char_array(tr_follow[gp_tr[g][t]]);
				printf("point %d",tr_point[gp_tr[g][t]]);
				printf("\n");
				t++;
			}
			t = 0;
			g++;
		}*/
		 
		temp = input_to_3d_char_array_from_file("gram");
		char **input = temp[0];
		int check = 0; //input array pointer
		char str[100][20];
		int str_head = 0; //string stack pointer
		int stack[100];
		stack[0] = 0;
		int stack_head = 0; // stack pointer
		int res;
		int xr = 0;
		while (p_attr_index(p_attr,input[check])!=-1) {
			res = p_table[stack[stack_head]][p_attr_index(p_attr,input[check])];
			if (res>0 && res!=1111) { //some shift
				stack[++stack_head] = res;
				strcpy(str[str_head++],input[check]);
				++check;
			}
			else if (res<=0 && res!=-1111) {
				res = -(res);
				int line_no = 0;
				while (gp_tr[stack[stack_head]][line_no]!=pow(2,13)-1) {
					if(check_if_two_lines_are_similar(text[res],tr[gp_tr[stack[stack_head]][line_no]]) &&
					   last_actual_index_of_2d_char_array(tr[gp_tr[stack[stack_head]][line_no]]) == 
					   tr_point[gp_tr[stack[stack_head]][line_no]])
						break;
					line_no++;
				}
				int pop = (tr_point[gp_tr[stack[stack_head]][line_no]] - 2) * 2;
				int tr_pos = gp_tr[stack[stack_head]][line_no];
				int k = 0;
				while (k<pop) {
					--stack_head;
					++k;
					if (k<pop) {
					--str_head;
					++k;
					}
				}
				strcpy(str[str_head++],tr[tr_pos][0]);
				stack[stack_head+1] = p_table[stack[stack_head]][p_attr_index(p_attr,str[str_head-1])];
				++stack_head;
			}
			else 
				break;
		}
		if (res == 1111)
			printf("\n\n\n\n""\t\t\t""\033[7m""\033[1m""\x1b[92m""ACCEPTED\n\n\n\n""\033[0m");
		else 	printf("\n\n\n\n""\t\t\t""\033[7m""\033[1m""\x1b[91m""REJECTED\n\n\n\n""\033[0m");
}
