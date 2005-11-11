%%
%% -----------------------------------------
%%     Central European Science Journals
%%     Universal Style File for LaTeX 2e
%%            http://www.cesj.com
%%
%%          Warsaw, December, 2002
%%              by m.osinski
%%
%%         based on a style file 
%%          by greg & m.osinski
%%
%% -----------------------------------------
%%

\def\cesj@fileversion{1.9.13}
\def\cesj@filedate{2003/03/05}
\def\cesj@logo{CESJ}
\def\cesj@journalLogo{\cesj@shortjournal}%
\def\cesj@cesj{Central European Science Journals}

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{cesj}[\cesj@filedate, \cesj@fileversion: \cesj@cesj]


\lineskip 1pt            % \lineskip is 1pt for all font sizes.
\normallineskip 1pt
\def\baselinestretch{1.4}
\baselineskip 12bp
\topskip 12bp

%%%%%%%%
\topmargin -20pt
\headheight 16bp
\headsep 24pt
\footskip 40bp
\parskip 0mm
\parindent 18bp


%%%%%%%%


\newif\if@TwoColumn
\newif\if@seceqn
\newif\if@secthm
\newif\if@nameyear
\newif\if@template
\newif\if@subsecthm
\@templatetrue
\DeclareOption{12pt}{\def\@argsize{12}}
\DeclareOption{11pt}{\def\@argsize{11}}
\DeclareOption{10pt}{\def\@argsize{10}}
\DeclareOption{a4paper}{%
  \setlength\paperheight {297mm}%
  \setlength\paperwidth  {210mm}%
}%
\DeclareOption{a5paper}
   {\setlength\paperheight {210mm}%
    \setlength\paperwidth  {148mm}}
\DeclareOption{b5paper}
   {\setlength\paperheight {250mm}%
    \setlength\paperwidth  {176mm}}
\DeclareOption{letterpaper}
   {\setlength\paperheight {11in}%
    \setlength\paperwidth  {8.5in}}
\DeclareOption{legalpaper}
   {\setlength\paperheight {14in}%
    \setlength\paperwidth  {8.5in}}
\DeclareOption{executivepaper}
   {\setlength\paperheight {10.5in}%
    \setlength\paperwidth  {7.25in}}
%
\DeclareOption{oneside}{\@twosidefalse \@mparswitchfalse}
\DeclareOption{twoside}{\@twosidetrue \@mparswitchtrue}
\@twocolumnfalse
\DeclareOption{onecolumn}{\@twocolumnfalse\@TwoColumnfalse}
\DeclareOption{twocolumn}{\@twocolumntrue\@TwoColumntrue}
\let\snm\relax \let\cty\relax \let\cny\relax\let\jr\relax
\@seceqnfalse                             % Default: equation numbering is not
\DeclareOption{seceqn}{\@seceqntrue}      % reset at beginning of each section
\@secthmfalse
\DeclareOption{secthm}{\@secthmtrue}
\@nameyearfalse
\DeclareOption{nameyear}{\@nameyeartrue}
\def\@blstr{1}
\DeclareOption{doublespacing}{\def\@blstr{1.7}}
\DeclareOption{reviewcopy}{\def\@blstr{1.7}}
\newif\if@symbold\@symboldfalse
\DeclareOption{symbold}{\@symboldtrue}
\newif\if@amsthm
% \firstpagelogo decides whether to use graphic (.eps) logo or text one
\DeclareOption{graphiclogo}{\def\firstpagelogo{\graphiclogo}}
\DeclareOption{textlogo}{\def\firstpagelogo{\textlogo}}
\DeclareOption{IJ}{%
  \def\cesj@journal{The Insight Journal}%
  \def\cesj@shortjournal{IJ}%
  \def\maketitle{\cesjmaketitle}
}%
\DeclareOption{CESJ}{%
  \def\cesj@journal{Central European Science Journals}%
  \def\cesj@shortjournal{CESJ}%
  \def\maketitle{\cesjmaketitle}
}%
\DeclareOption{CEJP}{%
  \def\cesj@journal{Central European Journal of Physics}%
  \def\cesj@shortjournal{CEJP}%
  \def\maketitle{\cejpmaketitle}
}%
\DeclareOption{CEJM}{%
  \def\cesj@journal{Central European Journal of Mathematics}%
  \def\cesj@shortjournal{CEJM}%
  \def\maketitle{\cesjmaketitle}
}%
\DeclareOption{NoArticleType}{%
  \def\@underLogoText{\ }
}%
\DeclareOption{CEJCH}{%
  \def\cesj@journal{Central European Journal of Chemistry}%
  \def\cesj@shortjournal{CEJC}%
  \def\maketitle{\cesjmaketitle}
}%
\DeclareOption{CEJC}{%
  \def\cesj@journal{Central European Journal of Chemistry}%
  \def\cesj@shortjournal{CEJC}%
  \def\maketitle{\cesjmaketitle}
}%
\DeclareOption{Review}{%
  \def\@underLogoText{Review article}%
}%
\DeclareOption{Research}{%
}%
\DeclareOption{Communication}{%
  \def\@underLogoText{Communication}%
}%
\DeclareOption{amsthm}{%
  \@amsthmtrue
  \@subsecthmfalse
}%
\@subsecthmfalse
\DeclareOption{subsecthm}{%
  \@subsecthmtrue
  \@amsthmfalse
}%
\DeclareOption{PS}{%
  \def\@logoext{eps}
}%
\DeclareOption{PDF}{%
  \def\@logoext{png}
}%

\ExecuteOptions{a4paper,12pt,textlogo,CESJ,amsthm,twoside,PS,NoArticleType}
\ProcessOptions
%\def\@pagenumprefix{}
\def\partname{Part}
\def\appendixname{Appendix}
\def\contentsname{Contents}
\def\listfigurename{List of Figures}
\def\listtablename{List of Tables}
\def\refname{References}
\def\indexname{Index}
\def\figurename{Fig.}
\def\tablename{Table}
\def\abstractname{Abstract}
\def\baselinestretch{1.4}
\newdimen\@frontmatterwidth
\def\@overcaptionskip{8\p@}
\def\@bibliosize{\small}
\def\@historysize{\small}
\def\@keywordsize{\small}

\def\@sizexii{%
  \@setsize\@sizexii{11.5pt}\xiipt\@xiipt
  \abovedisplayskip 12pt plus3pt minus7pt 
  \belowdisplayskip \abovedisplayskip 
  \abovedisplayshortskip  \z@ plus3pt
  \belowdisplayshortskip  6.5pt plus3.5pt minus3pt
}%

\def\@sizexi{%
  \@setsize\@sizexi{11pt}\xipt\@xipt
  \abovedisplayskip 12pt plus3pt minus7pt 
  \belowdisplayskip \abovedisplayskip 
  \abovedisplayshortskip  \z@ plus3pt
  \belowdisplayshortskip  6.5pt plus3.5pt minus3pt
}%

\ifnum \@argsize = 12 
  \def\normalsize{\@sizexii}
\else 
  \def\normalsize{\@sizexi}
\fi

\def\@abstractsize{%
  \@setsize\@abstractsize{10pt}\xipt\@xipt}%

\def\@tocsize{%
  \@setsize\@tocsize{10pt}\xipt\@xipt}%
 
\def\small{%
  \@setsize\small{5pt}\xviipt\@xiipt
  \abovedisplayskip 10pt plus2pt minus5pt 
  \belowdisplayskip \abovedisplayskip 
  \abovedisplayshortskip \z@ plus3pt
  \belowdisplayshortskip 6pt plus3pt minus3pt
  \def\@listi{%
    \topsep 6pt plus 2pt minus 2pt\parsep 3pt plus 2pt minus 1pt
    \itemsep \parsep
  }%
}%

% fontsize used in captions (figure and table environments)
\def\captionfontsize{%
  \@setsize\captionfontsize{8pt}\xpt\@xpt
  \abovedisplayskip 8pt plus2pt minus5pt 
  \belowdisplayskip \abovedisplayskip 
  \abovedisplayshortskip \z@ plus3pt
  \belowdisplayshortskip 6pt plus3pt minus3pt
  \def\@listi{%
    \topsep 6pt plus 2pt minus 2pt
    \parsep 3pt plus 2pt minus 1pt
    \itemsep \parsep
  }%
}%

\def\@captionsize{\captionfontsize}
\def\@tablecaptionsize{\@captionsize}
\def\@figurecaptionsize{\@captionsize}
\def\@tablesize{%
  \@setsize\@tablesize{10pt}\xipt\@xipt
  \abovedisplayskip 10pt plus2pt minus5pt \belowdisplayskip
  \abovedisplayskip \abovedisplayshortskip \z@ plus3pt
  \belowdisplayshortskip 6pt plus3pt minus3pt
  \def\@listi{%
    \topsep 6pt plus 2pt minus 2pt
    \parsep 3pt plus 2pt minus 1pt
    \itemsep \parsep
  }%
}%


\def\@headfontsize{%
  \@setsize\@headfontsize{12pt}\xpt\@xpt}

\def\footnotesize{%
  \@setsize\footnotesize{10pt}\xpt\@xpt
  \abovedisplayskip 10pt plus2pt minus5pt%
  \belowdisplayskip \abovedisplayskip
  \abovedisplayshortskip  \z@ plus3pt%
  \belowdisplayshortskip  6pt plus3pt minus3pt
  \def\@listi{%
    \topsep 6pt plus 2pt minus 2pt
    \parsep 3pt plus 2pt minus 1pt
    \itemsep \parsep
  }%
}%

\def\scriptsize{\@setsize\scriptsize{9.5pt}\viiipt\@viiipt}

\def\tiny{\@setsize\tiny{7pt}\vipt\@vipt}

\def\large{\@setsize\large{14pt}\xivpt\@xivpt}

\def\Large{\@setsize\Large{14pt}\xviipt\@xviipt}

\def\LARGE{\@setsize\LARGE{20pt}\xxpt\@xxpt}

\def\huge{\@setsize\huge{22pt}\xxvpt\@xxvpt}

\def\Huge{\@setsize\Huge{25pt}\xxvpt\@xxvpt}

\abovedisplayskip 11pt plus3pt minus6pt%
\belowdisplayskip \abovedisplayskip
\abovedisplayshortskip = 0pt plus3pt%
\belowdisplayshortskip  6.5pt plus3.5pt minus3pt

\def\@listi{%
  \parsep 4.5pt plus 2pt minus 1pt
  \itemsep \parsep
  \topsep 9pt plus 3pt minus 5pt
}%

\normalsize                                % Choose the normalsize font.
\newdimen\@bls                              % Several dimensions are
\@bls=\baselineskip                         % expressed in terms of this.
\oddsidemargin   0pt    %   Left margin on odd-numbered pages.
\evensidemargin  0pt    %   Left margin on even-numbered pages.
\marginparwidth  2pt    %   \@Width of marginal notes.
%\marginparsep 20\p@          % Horizontal space between outer margin and
                            % marginal note

\newcommand\section{%
  \@startsection {section}{1}{\z@}{-3.5ex plus -1ex minus -.2ex}%
  {2.3ex plus .2ex}{\large\bf}%
}%

\newcommand\subsection{%
  \@startsection{subsection}{2}{\z@}{-3.5ex plus -1ex minus -.2ex}%
    {2.3ex plus .2ex}{\reset@font\large}%
}%

\newcommand\subsubsection{%
  \@startsection {subsubsection}{3}{\z@}{-3.25ex plus -1ex minus-.2ex}%
    {1.5ex plus.2ex}{\reset@font\small}%
}%

\newcommand\paragraph{\@startsection{paragraph}{4}{\z@}{3.25ex \@plus
  2ex \@minus 0.2ex}{-1em}{\normalsize\bfseries}}

\setcounter{secnumdepth}{4}
\def\half@em{\hskip 0.5em}
\def\lb@part{PART \thepart.\half@em}
  \def\lb@empty@part{PART \thepart}
\def\lb@section{\thesection.\half@em}
  \def\lb@empty@section{\thesection}
\def\lb@subsection{\thesubsection.\half@em}
  \def\lb@empty@subsection{\thesubsection}
\def\lb@subsubsection{\thesubsubsection.\half@em}
  \def\lb@empty@subsubsection{\thesubsubsection}
\def\lb@paragraph{\theparagraph.\half@em}
  \def\lb@empty@paragraph{\theparagraph}
\def\lb@subparagraph{\thesubparagraph.\half@em}
  \def\lb@empty@subparagraph{\thesubparagraph}
\def\app@number#1{\setcounter{#1}{0}%
  \@addtoreset{#1}{section}%
  \@namedef{the#1}{\thesection.\arabic{#1}}}
\def\appendix{\@ifstar{\appendix@star}{\appendix@nostar}}
\def\appendix@nostar{%
  \def\lb@section{\appendixname\ \thesection.\half@em}
  \def\lb@empty@section{\appendixname\ \thesection}
  \setcounter{section}{0}\def\thesection{\Alph{section}}%
  \setcounter{subsection}{0}%
  \setcounter{subsubsection}{0}%
  \setcounter{paragraph}{0}%
  \app@number{equation}\app@number{figure}\app@number{table}}
\def\appendix@star{%
  \def\lb@section{\appendixname}\let\lb@empty@section\lb@section
  \setcounter{section}{0}\def\thesection{\Alph{section}}%
  \setcounter{subsection}{0}%
  \setcounter{subsubsection}{0}%
  \setcounter{paragraph}{0}%
  \app@number{equation}\app@number{figure}\app@number{table}}
\def\ack{\section*{Acknowledgements}%
  \addtocontents{toc}{\protect\vspace{6pt}}%
  \addcontentsline{toc}{section}{Acknowledgements}%
}
\@namedef{ack*}{\par\vskip 3.0ex \@plus 1.0ex \@minus 1.0ex}
\let\endack\par
\@namedef{endack*}{\par}
\newdimen\labelwidthi
\newdimen\labelwidthii
\newdimen\labelwidthiii
\newdimen\labelwidthiv
\def\normal@labelsep{0.5em}
\labelsep\normal@labelsep
\settowidth{\labelwidthi}{(iii)}
\settowidth{\labelwidthii}{(d)}
\settowidth{\labelwidthiii}{(iii)}
\settowidth{\labelwidthiv}{(M)}
\leftmargini\labelwidthi    \advance\leftmargini\labelsep
\leftmarginii\labelwidthii  \advance\leftmarginii\labelsep
\leftmarginiii\labelwidthiii \advance\leftmarginiii\labelsep
\leftmarginiv\labelwidthiv  \advance\leftmarginiv\labelsep
\def\setleftmargin#1#2{\settowidth{\@tempdima}{#2}\labelsep\normal@labelsep
  \csname labelwidth#1\endcsname\@tempdima
  \@tempdimb\@tempdima \advance\@tempdimb\labelsep
  \csname leftmargin#1\endcsname\@tempdimb}
\def\@listI{\leftmargin\leftmargini
  \labelwidth\labelwidthi \labelsep\normal@labelsep
  \topsep \z@ \partopsep\z@ \parsep\z@ \itemsep\z@
  \listparindent 1em}
\def\@listii{\leftmargin\leftmarginii
  \labelwidth\labelwidthii \labelsep\normal@labelsep
  \topsep\z@ \partopsep\z@ \parsep\z@ \itemsep\z@
  \listparindent 1em}
\def\@listiii{\leftmargin\leftmarginiii
  \labelwidth\labelwidthiii \labelsep\normal@labelsep
  \topsep\z@ \partopsep\z@ \parsep\z@ \itemsep\z@
  \listparindent 1em}
\def\@listiv{\leftmargin\leftmarginiv
  \labelwidth\labelwidthiv \labelsep\normal@labelsep
  \topsep\z@ \partopsep\z@ \parsep\z@ \itemsep\z@
  \listparindent 1em}
\let\@listi\@listI
\@listi
\def\left@label#1{{#1}\hss}
\def\right@label#1{\hss\llap{#1}}
\def\thick@label#1{\hspace\labelsep #1}
\newcount\@maxlistdepth
\@maxlistdepth=4
\def\labelitemi{$\bullet$}          \def\labelitemii{$\cdot$}
\def\labelenumi{(\theenumi)}        \def\theenumi{\arabic{enumi}}
\def\labelenumii{(\alph{enumii})}   \def\theenumii{\theenumi\alph{enumii}}
\def\labelenumiii{(\roman{enumiii})}\def\theenumiii{\theenumii.\roman{enumiii}}
\def\labelenumiv{(\Alph{enumiv})}   \def\theenumiv{\theenumiii.\Alph{enumiv}}
\def\verse{\let\\=\@centercr
  \list{}{\itemsep\z@
  \itemindent \z@
  \listparindent\z@
  \rightmargin 1em
  \leftmargin \rightmargin}\item[]}
\let\endverse\endlist
\def\quotation{\list{}{\itemindent\z@
 \leftmargin 1em \rightmargin \z@
  \parsep \z@ \@plus 1pt}\item[]}
\let\endquotation=\endlist
\def\quote{\list{}{\itemindent\z@
   \leftmargin 1em \rightmargin \z@}%
\item[]}
\let\endquote=\endlist
\def\descriptionlabel#1{\hspace\labelsep \bfseries #1}
\def\description{\list{}{\labelwidth\z@
  \leftmargin 1em \itemindent-\leftmargin
  \let\makelabel\descriptionlabel}}
\let\enddescription\endlist
\def\@atfmtname{atlplain}
\ifx\fmtname\@atfmtname
 \def\neq{\not\nobreak\mkern -2mu =}%
 \let\ne\neq
\fi
\def\operatorname#1{\mathop{\mathrm{#1}}\nolimits}
\def\lefteqn#1{\hbox to\z@{$\displaystyle {#1}$\hss}}
\newskip\eqnbaselineskip % Standard interline spacing in an {eqnarray}
\jot=2\p@
\newskip\eqnlineskip     % Minimal space between the bottom of
                         % a line and the top of the next line.
\newskip\eqntopsep                    % Extra vertical space, in addition to
 \eqntopsep 12\p@ \@plus 2\p@ \@minus 2\p@ %\parskip, added above and below
\newdimen\eqnarraycolsep            % Half the space between columns
\eqnarraycolsep 1\p@                 % in an \eqnarray.

\eqnbaselineskip=14\p@  \eqnlineskip=2\p@
\newdimen\mathindent
\if@TwoColumn
  \mathindent 0em
\else
  \mathindent 2em
\fi
\def\[{\relax\ifmmode\@badmath
  \else%\bgroup removed on request from BW (1993-05-17)
  \@beginparpenalty\predisplaypenalty
  \@endparpenalty\postdisplaypenalty
  \begin{trivlist}\@topsep \eqntopsep       % used by first \item
   \@topsepadd \eqntopsep                   % used by \@endparenv
  \item[]\leavevmode
   \hbox to\linewidth\bgroup$ \displaystyle
  \hskip\mathindent\bgroup\fi}
\def\]{\relax\ifmmode \egroup $\hfil \egroup
  \end{trivlist}% \egroup removed on request from BW (1993-05-17)
  \else \@badmath \fi}

\def\equation{$$\refstepcounter{equation}}
\def\endequation{\eqno \hbox{\@eqnnum}$$\@ignoretrue}
\def\@eqnnum{{\normalfont \normalcolor (\theequation)}}

\def\eqnarray{%
   \stepcounter{equation}%
   \def\@currentlabel{\p@equation\theequation}%
   \global\@eqnswtrue
   \m@th
   \global\@eqcnt\z@
   \tabskip\@centering
   \let\\\@eqncr
   $$\everycr{}\halign to\displaywidth\bgroup
       \hskip\@centering$\displaystyle\tabskip\z@skip{##}$\@eqnsel
      &\global\@eqcnt\@ne\hskip \tw@\arraycolsep \hfil${##}$\hfil
      &\global\@eqcnt\tw@ \hskip \tw@\arraycolsep
         $\displaystyle{##}$\hfil\tabskip\@centering
      &\global\@eqcnt\thr@@ \hb@xt@\z@\bgroup\hss##\egroup
         \tabskip\z@skip
      \cr
}
\def\endeqnarray{%
      \@@eqncr
      \egroup
      \global\advance\c@equation\m@ne
   $$\@ignoretrue
}
\let\@eqnsel=\relax
\def\nonumber{\global\@eqnswfalse}
\def\@eqncr{%
   {\ifnum0=`}\fi
   \@ifstar{%
      \global\@eqpen\@M\@yeqncr
   }{%
      \global\@eqpen\interdisplaylinepenalty \@yeqncr
   }%
}
\def\@yeqncr{\@testopt\@xeqncr\z@skip}
\def\@xeqncr[#1]{%
   \ifnum0=`{\fi}%
   \@@eqncr
   \noalign{\penalty\@eqpen\vskip\jot\vskip #1\relax}%
}
\def\@@eqncr{\let\reserved@a\relax
    \ifcase\@eqcnt \def\reserved@a{& & &}\or \def\reserved@a{& &}%
     \or \def\reserved@a{&}\else
       \let\reserved@a\@empty
       \@latex@error{Too many columns in eqnarray environment}\@ehc\fi
     \reserved@a \if@eqnsw\@eqnnum\stepcounter{equation}\fi
     \global\@eqnswtrue\global\@eqcnt\z@\cr}
\let\@seqncr=\@eqncr
\@namedef{eqnarray*}{\def\@eqncr{\nonumber\@seqncr}\eqnarray}
\@namedef{endeqnarray*}{\nonumber\endeqnarray}
\def\lefteqn#1{\rlap{$\displaystyle #1$}}

\def\pre@coli{\hskip\@centering}              \def\post@coli{}
\def\pre@colii{\hskip 2\eqnarraycolsep \hfil} \def\post@colii{\hfil}
\def\pre@coliii{\hskip 2\eqnarraycolsep}      \def\post@coliii{\hfil}
\arraycolsep 2\p@         % Half the space between columns in array environment.
\tabcolsep 6\p@           % idem in tabular environment.
\def\arraystretch{1.5}   % More vertical space in tables
\arrayrulewidth 0.4\p@    % \@Width of rules and space between adjacent
\doublerulesep 2\p@       % rules in any of these two environments.
\newdimen\rulepreskip \newdimen\rulepostskip
\rulepreskip=4\p@      \rulepostskip=6\p@
\tabbingsep \labelsep   % Space used by the \' command. '(See LaTeX{} manual.)
\skip\@mpfootins = \skip\footins
\fboxsep = 7\p@    % Space left between box and text by \fbox and \framebox.
\fboxrule = 0.4\p@ % \@Width of rules in box made by \fbox and \framebox.
\newcounter{section}
\newcounter{subsection}[section]
\newcounter{subsubsection}[subsection]
\newcounter{paragraph}[subsubsection]
\newcounter{subparagraph}[paragraph]
\if@seceqn
 \@addtoreset{equation}{section}
 \def\theequation{\arabic{section}.\arabic{equation}}
\else
  \def\theequation{\arabic{equation}}
\fi
\def\thesection      {\arabic{section}}
\def\thesubsection   {\thesection.\arabic{subsection}}
\def\thesubsubsection{\thesubsection.\arabic{subsubsection}}
\def\theparagraph    {\thesubsubsection.\arabic{paragraph}}
\def\thesubparagraph {\theparagraph.\arabic{subparagraph}}
\@addtoreset{section}{part} % reset section numbers at beginning of part

\DeclareOldFontCommand{\rm}{\normalfont\rmfamily}{\mathrm}
\DeclareOldFontCommand{\sf}{\normalfont\sffamily}{\mathsf}
\DeclareOldFontCommand{\tt}{\normalfont\ttfamily}{\mathtt}
\DeclareOldFontCommand{\bf}{\normalfont\bfseries}{\mathbf}
\DeclareOldFontCommand{\it}{\normalfont\itshape}{\mathit}
\DeclareOldFontCommand{\sl}{\normalfont\slshape}{\@nomath\sl}
\DeclareOldFontCommand{\sc}{\normalfont\scshape}{\@nomath\sc}
\RequirePackage{latexsym}
\RequirePackage{graphicx}
\def\qed{\relax\ifmmode\hskip2em \Box\else\unskip\nobreak\hskip1em $\Box$\fi}

% theorem, proof etc implemented by \subsection{} !!!
\if@subsecthm
  % numbered
  \renewcommand\newtheorem{\@ifstar{\@unnumnewtheorem}{\@numnewtheorem}}
  \def\@numnewtheorem#1{%
    \@ifnextchar[{\@mythm{#1}}{\@mythm{#1}[foo]}}
  \def\@mythm#1[#2]#3{%
    \global\@namedef{#1}{\@mybthm{#3}}%
    \global\@namedef{end#1}{\@myendtheorem}}%}}
  \def\@mybthm#1{%
    \@ifnextchar[{\@myythm{#1}}{\@myxthm{#1}}}
  \def\@myxthm#1{%
    \@mybegintheorem{#1}}
  \def\@myythm#1[#2]{%
    \@myopargbegintheorem{#1}{#2}}
  \def\@mybegintheorem#1{\subsection{#1}}
  \def\@myopargbegintheorem#1#2{\subsection{#1\ (#2)}}
  \def\@myendtheorem{}
  % unnumbered
  \def\@unnumnewtheorem#1#2{%
    \global\@namedef{#1}{\@ifnextchar[{\@unmyythm{#2}}{\@unmybegintheorem{#2}}}
    \global\@namedef{end#1}{\@myendtheorem}}%
  \def\@unmyythm#1[#2]{%
    \@unmyopargbegintheorem{#1}{#2}}
  \def\@unmybegintheorem#1{\subsection*{#1}}
  \def\@unmyopargbegintheorem#1#2{\subsection*{#1\ (#2)}}
\fi

\if@subsecthm
  \newcounter{thm}[section]
  \newtheorem*{proof*}{Proof}
\else
% implementation of theorem, .. similar to amsthm
% however, proof is like all the others
  \RequirePackage{amsthm}
  \def\th@cesj{%
    \def\thm@preskip{1.2em}
    \def\thm@postskip{1.2em}
    \normalfont
  }%
  \theoremstyle{cesj}
% thanks to "thm" we can give a unified "interface" for both
% .sty (Latex2.09) and .cls (latex2e)
  \newtheorem{thm}{Theorem}[section]
  \let\proof\undefine
  \newtheorem*{proof*}{Proof}
\fi

\newtheorem{theorem}[thm]{Theorem}
\newtheorem{lemma}[thm]{Lemma}
\newtheorem{claim}[thm]{Claim}
\newtheorem{axiom}[thm]{Axiom}
\newtheorem{fact}[thm]{Fact}
\newtheorem{proof}[thm]{Proof}
\newtheorem{hypo}[thm]{Hypothesis}
\newtheorem{assumption}[thm]{Assumption}
\newtheorem{definition}[thm]{Definition}
\newtheorem{problem}[thm]{Problem}
\newtheorem{example}[thm]{Example}
\newtheorem{remark}[thm]{Remark}
\newtheorem{corollary}[thm]{Corollary}
\newtheorem{proposition}[thm]{Proposition}
\newtheorem{alg}[thm]{Algoritheorem}


\long\def\@makealgocaption#1#2{\vskip 2ex \small
  \hbox to \hsize{\parbox[t]{\hsize}{{\bfseries #1.} #2}}}
\newcounter{algorithm}
\def\thealgorithm{\@arabic\c@algorithm}
\def\fps@algorithm{tbp}
\def\ftype@algorithm{4}
\def\ext@algorithm{lof}
\def\fnum@algorithm{Algorithm \thealgorithm}
\def\algorithm{\let\@makecaption\@makealgocaption\@float{algorithm}}
\let\endalgorithm\end@float
\def\@pnumwidth{2.55em}
\def\@tocrmarg{2.55em \@plus 5em}
\def\@dotsep{-2.5}
\setcounter{tocdepth}{2}
\newcommand\listoffigures{%
    \section*{\listfigurename
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}}%
    \@starttoc{lof}%
    }
\newcommand*\l@figure{\@dottedtocline{1}{1.5em}{2.3em}}
\newcommand\listoftables{%
    \section*{\listtablename
      \@mkboth{%
          \MakeUppercase\listtablename}{\MakeUppercase\listtablename}}%
    \@starttoc{lot}%
    }
\let\l@table\l@figure
\def\tableofcontents{%
  \section*{\contentsname
    \@mkboth{%
      \MakeUppercase\contentsname}{\MakeUppercase\contentsname}}%
      \@starttoc{toc}%
}%
\newcommand*\l@section{\@dottedtocline{1}{1.5em}{2.3em}}
\newcommand*\l@subsection{\@dottedtocline{2}{3.8em}{2.3em}}
\newcommand*\l@subsubsection{\@dottedtocline{3}{7.0em}{3.2em}}
\newcommand*\l@paragraph{\@dottedtocline{4}{10.0em}{4.1em}}
\newcommand*\l@subparagraph{\@dottedtocline{5}{13em}{5em}}
\def\@dotsep{2000}
\def\thebibliography{%
  \@startsection{section}{1}{\z@}{20\p@ \@plus 8\p@ \@minus 4pt}
  {\@bls}{\large\bfseries}*{\refname}%
\addcontentsline{toc}{section}{\refname}%
  \@thebibliography}
\let\endthebibliography=\endlist
\def\@thebibliography#1{\@bibliosize
  \list{\@biblabel{\arabic{enumiv}}}{\settowidth\labelwidth{\@biblabel{#1}}
  \if@nameyear
    \labelwidth\z@ \labelsep\z@ \leftmargin\parindent
    \itemindent-\parindent
  \else
    \labelsep 3\p@ \itemindent\z@
    \leftmargin\labelwidth \advance\leftmargin\labelsep
\fi
     \itemsep 0.3\@bls \@plus 0.1\@bls \@minus 0.1\@bls
    \usecounter{enumiv}\let\p@enumiv\@empty
    \def\theenumiv{\arabic{enumiv}}}%
    \tolerance\@M
   \hyphenpenalty\@M
     \hbadness5000 \sfcode`\.=1000\relax}
\newcommand\newblock{\hskip .11em\@plus.33em\@minus.07em}
\if@nameyear
  \def\@biblabel#1{}
\else
 \def\@biblabel#1{[#1]\hskip \z@ \@plus 1filll}
\fi
\let\make@bb@error\relax
\def\@mkbberr{\def\bibitem{\ClassError{elsart}%
    {Bibitem after note}%
    {You are using a bibitem after a note in a subbibitems
      environment;\MessageBreak
      note should the last item in a subbibitems environment}}}
\def\@itemnote{\make@bb@error\item[]}
\def\mk@noitemnote{\ifx\@tempa\note \let\note\@noitemnote \fi}
\def\@noitemnote{\let\note\@itemnote}
\AtBeginDocument{%
  \let\nopeek@bibitem\@bibitem
  \let\nopeek@lbibitem\@lbibitem
  \def\@bibitem#1{\let\note\@itemnote\nopeek@bibitem{#1}%
    \futurelet\@tempa\mk@noitemnote}
  \def\@lbibitem[#1]#2{\let\note\@itemnote\nopeek@lbibitem[#1]{#2}%
    \futurelet\@tempa\mk@noitemnote}
}

\def\footnote{%
  \@ifnextchar[{\@xfootnote}{%
    \refstepcounter {\@mpfn}%
    \protected@xdef\@thefnmark{\thempfn}\@footnotemark\@footnotetext%
  }%
}%
\def\footnotemark{%
  \@ifnextchar[{%
    \@xfootnotemark}{%
    \refstepcounter{footnote}\xdef\@thefnmark{\thefootnote}\@footnotemark%
  }%
}%
\def\footnoterule{\kern-3\p@
  \hrule \@width 3pc               % The \hrule has default \@height of 0.4pt.
  \kern 2.6\p@}
\def\thempfootnote{\alph{mpfootnote}}

\def\@mpmakefnmark{\,\hbox{$^{\mathrm{\@thefnmark}}$}}
\long\def\@mpmakefntext#1{\noindent
                     \hbox{$^{\mathrm{\@thefnmark}}$} #1}
\def\fn@presym{}
\long\def\@makefntext#1{\noindent\hbox to 1em
  {$^{\fn@presym\mathrm{\@thefnmark}}$\hss}#1}

\DeclareRobustCommand*\cal{\@fontswitch\relax\mathcal}

% from CEJM_2002
\def\@title{}

%\def\@authors{}
\def\@author{}
\def\@issue{}
\def\@year{(2003)}
\def\@received{}
\def\@revised{}
\def\@accepted{}
\def\@MSC{}
\def\@PACS{}
\def\@classification{}
\def\@leftmark{}
\def\@rightmark{}

%\def\@authoraddress{}

\def\title#1{\gdef\@title{{\Large\bf\noindent\ignorespaces#1\vskip 0.2pt}}}
  
\def\author#1{\def\@author{\large #1}}

\def\abstract#1{\def\@abstract{\@abstractsize #1}}

\def\keyword#1{\def\@keyword{\@abstractsize \it Keywords:\ \@abstractsize #1}}

\def\classification#1#2{\def\@classification{%
  \eject
  {\@abstractsize \it #1:\ \@abstractsize #2}}%
}%

\def\MSC#1{\classification{MSC (2000)}{#1}}

%\def\MSC#1{\def\@MSC{%
%  \eject
%  {\@abstractsize \it MSC (2000):\ \@abstractsize #1}}%
%}%

\def\PACS#1{\classification{PACS (2000)}{#1}}

%\def\PACS#1{\def\@classification{%
%  \eject
%  {\@abstractsize \it PACS (2000):\ \@abstractsize #1}}%
%}%

\def\issue#1{\def\@issue{#1}}
\def\email#1{\thanks{#1}}
\def\year#1{\def\@year{(#1)}}

\def\leftmark#1{\def\@leftmark{#1 / \cesj@journal ~\@issue ~\@year}}
\def\rightmark#1{\def\@rightmark{#1 / \cesj@journal ~\@issue ~\@year~}}

\def\received#1{\def\@received{\@abstractsize {Received #1}}}
\def\revised#1{\def\@revised{\@abstractsize {;~revised #1}}}
\def\accepted#1{\def\@accepted{\@abstractsize {;~accepted #1}}}

\def\@copyright{\textcircled{c} Central European Science Journals. All rights reserved.}

\normalsize

\def\@fhead{%
  \parbox[t]{\textwidth}{\hfill{} \\
  \null \hfill{}}
}%

\def\copyrightline{\footnotesize \@copyright \hfill\hbox{}\par}

\def\copyrightfoot{%
%  \advance\leftskip -16pt
  \leavevmode \lower 50pt \vbox {\bf \copyrightline{}}
}%

\def\b{\hspace*{1mm}}

%%%%%%%%%%%%%%%
\def\@headR{
    %
  \normalsize

  \parbox[t]{8.7cm}{%
  \vspace*{-18ptcm}%
  }%
}%

\def\@headline{

}

%%%%%%%%%%%%%

\def\ps@titlepage{%
  \topmargin -22pt
%  \topmargin 0bp
  \headheight 42bp
  \headsep -18bp
  \let\@oddhead\@headline
  \let\@evenhead\@headline
%  \let\@oddfoot\@fhead
% only in template !
  \if@template
  \else
    \let\@oddfoot\copyrightfoot%
  \fi
}%

\def\ps@myheadings{%
  \let\@mkboth\@gobbletwo
  \def\@evenhead{%
    \underline{%
      \parbox[c]{\textwidth}{%
        \rm{\@headfontsize \thepage}\hfill{}
        \rule[-3.4pt]{0pt}{10pt}\rm{\@headfontsize\@leftmark ~\thefirstpage--\thelastpage }%
      }%
    }%
  }%
  \def\@evenfoot{}
  \def\@oddhead{%
    \underline{%
      \parbox[c]{\textwidth}{%
        {\rm{\@headfontsize \@rightmark ~\thefirstpage--\thelastpage }}
        \hfill{}\rule[-3.4pt]{0pt}{10pt}{\@headfontsize\thepage}%
      }%
    }%
  }%
  \def\@oddfoot{}
}%


%%%%%<<<<  institute  >>>>

\newcounter{@inst}
\newcounter{@przec}

\def\institute#1{\gdef\@institute{#1}}

\def\inst#1{%
  \unskip
  \setcounter{@przec}{0}%
  \@for\@x:=#1\do%
   {%
    \ifnum\c@@przec=0\relax%
    \else $^,$%
    \fi%
    \@ifundefined{r@\@x}{%
        \unskip$^{\@x}$}%
        {%
    \unskip$^{\ref{\@x}}$}%
    \addtocounter{@przec}{1}%
   }%
}%

\def\institutename{%
  \par
  \begingroup
    \parskip=\z@
    \parindent=\z@
    \setcounter{@inst}{1}%
    \def\and{%
    \vspace{4pt}
      \par\stepcounter{@inst}%
      \noindent$^{\the@inst}$\enspace\ignorespaces
    }%
%%
    \setbox0=\vbox{\def\thanks##1{}\@institute}%
    \ifnum\c@@inst=1\relax
    \else
      \setcounter{footnote}{\c@@inst}%
      \setcounter{@inst}{1}%
      \def\@currentlabel{\the@inst}
      \noindent$^{\the@inst}$\enspace
    \fi
    \ignorespaces
%%
    \@institute\par
  \endgroup%
}%

\renewcommand{\thefootnote}{\fnsymbol{footnote}}


% logos
% used to avoid using .eps logos
\def\textlogo{%
  \begin{figure}
    \vspace*{8mm}%
    \centerline{%
      \makebox[40mm]{\@headfontsize \cesj@shortjournal ~\@issue ~\@year ~\thefirstpage--\thelastpage }%
      \hfill
      \makebox[60mm]{\captionfontsize\it \cesj@journal}
    }%
  \vspace{0.0cm}
% line below is needed for ,,compatibility'' with graphic version
  \rightline{\ }%
  \end{figure}
}%


\def\graphiclogo{
  \begin{figure}
    \vspace*{-0.5cm}%
    \centerline{%
      \includegraphics[height=23.67mm,width=54.88mm,angle=0,scale=0.9]{\cesj@logo.\@logoext}
      \hfill
      \includegraphics[height=23.67mm,width=54.88mm,angle=0,scale=0.9]{\cesj@journalLogo.\@logoext}%
    }%
    \vspace{-0.2cm}
    \rightline {\@headfontsize \cesj@shortjournal ~\@issue ~\@year ~\thefirstpage--\thelastpage \ }%
    \vspace{-0.1cm}
    \rightline{\small\it{}\@underLogoText\ }%
    \vspace{-0.4cm}
  \end{figure}
}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  Make Title

\def\cesjmaketitle{%

  \vspace*{-1cm}%
%  \thispagestyle{empty}
  \thispagestyle{titlepage}
%
% to access the number of the first page 
  \firstpagelogo%
  \normalsize
%
  \setcounter{page}{1}
%
  \begin{center}
    \noindent
    \begin{minipage}[t]{\textwidth}
      \null
      \vspace{1.2cm}
      \hyphenpenalty=10000
      %\flushleft
      \begin{center}
        {\@title}\par
      \end{center}
      \vspace{1.0cm}
      \begin{center}
        {\@author}\par
      \end{center}
      \vskip 1em%
      \@ifundefined{@institute}{\relax}{\small\center{\it\institutename}}
          %
          %
      \let\abstractwidth\textwidth
         %
      \addtolength{\abstractwidth}{0mm}%!!!!!!
      \noindent\hspace*{0mm}%!!!!!!
      {\begin{minipage}[t]{\abstractwidth}
        \eject
        \vspace{1cm}
        \rightline {\@received \@revised \@accepted}
        \vspace{0.2cm}
        \rule [0mm]{\abstractwidth}{0.3pt}
%        \vspace{0.1cm}
        \eject
        {\@abstractsize \bf Abstract:}
%        \eject
 %       \vspace{12bp}
        \@abstract 
% 	\newline {\@abstractsize \@copyright}
        \vspace{0.4cm}
        \eject
        \@keyword
%        \@MSC
%        \@PACS
	\@classification
        \eject
        \vspace{-0.2cm}
        \rule [0mm]{\abstractwidth}{1pt}
        \eject
        \vspace{0.3cm}
      \end{minipage}}

      \par \vspace*{0pt}
      \let\maketitle\relax 
      \let\@maketitle\relax

    \end{minipage}
    \normallineskip 0.5mm
    \@thanks
    \let\@thanks\relax
    \normalsize
  \end{center}
}%

\def\cejpmaketitle{%

  \vspace*{-1cm}%
%  \thispagestyle{empty}
  \thispagestyle{titlepage}
%
% to access the number of the first page 
  \firstpagelogo%
  \normalsize
%
  \setcounter{page}{1}
%
  \begin{center}
    \noindent
    \begin{minipage}[t]{\textwidth}
      \null
      \vspace{1.2cm}
      \hyphenpenalty=10000
      %\flushleft
      \begin{flushleft}
        {\@title}\par
      \end{flushleft}
      \vspace{1.0cm}
	    %
	    %
      \let\abstractwidth\textwidth
	   %
      \begin{minipage}[t]{\abstractwidth}
      \addtolength{\abstractwidth}{-1cm}%
      \advance\leftskip 1cm
	{\@author}\par
	\vskip 1em%
	\@ifundefined{@institute}{\relax}{\small{\it\institutename}}
	\noindent\hspace*{0mm}%!!!!!!
	  \eject
	  \vspace{0.7cm}
	  \rightline{\@received \@revised \@accepted}
	  \vspace{0.2cm}
	  \rule [0mm]{\abstractwidth}{0.3pt}
	  \eject
	  {\@abstractsize \bf Abstract:}
  %        \eject
   %       \vspace{12bp}
	  \@abstract 
	  \newline {\@abstractsize \@copyright}
	  \vspace{0.4cm}
	  \eject
	  \@keyword
%	  \@MSC
%	  \@PACS
	  \@classification
	  \eject
	  \vspace{-0.2cm}
	  \rule [0mm]{\abstractwidth}{1pt}
	  \eject
	  \vspace{0.3cm}
      \end{minipage}

      \par \vspace*{0pt}
      \let\maketitle\relax 
      \let\@maketitle\relax

    \end{minipage}
    \normallineskip 0.5mm
    \@thanks
    \let\@thanks\relax
    \normalsize
  \end{center}
}%




\setcounter{topnumber}{5}
\def\topfraction{0.99}
\def\textfraction{0.05}
\def\floatpagefraction{0.9}
\setcounter{bottomnumber}{5}
\def\bottomfraction{0.99}
\setcounter{totalnumber}{10}
\def\dbltopfraction{0.99}
\def\dblfloatpagefraction{0.8}
\setcounter{dbltopnumber}{5}
\long\def\@caption#1[#2]#3{
    \par\addcontentsline{\csname ext@#1\endcsname}{#1}
      {\protect\numberline{\csname the#1\endcsname}{\ignorespaces #2}}
    \begingroup
      \@parboxrestore
      \let\captionwidth\textwidth
      \addtolength{\captionwidth}{-36bp}
      \begin{center}
        \begin{minipage}{\captionwidth}
          {\baselineskip 10bp \@captionsize 
	    \@makecaption{\csname fnum@#1\endcsname}
          {\ignorespaces #3}\par}
        \end{minipage}
      \end{center}
    \endgroup
}

\long\def\@makefigurecaption#1#2{%
  \vskip 10\p@
  \setbox\@tempboxa\hbox{{\bf #1} #2}%
  \ifdim \wd\@tempboxa >\hsize
    {\bf #1} #2\par
  \else
    \hbox to\hsize{\hfil\box\@tempboxa\hfil}%
  \fi}

\def\@maketablecaption{\@makefigurecaption}

\def\@makecaption{\@makefigurecaption}

\def\conttablecaption{\par \begingroup \@parboxrestore \normalsize
  \@makecaption{\fnum@table\,---\,continued}{}\par
  \vskip-1pc \endgroup}
\def\contfigurecaption{\vskip-1pc \par \begingroup \@parboxrestore
  \@captionsize
  \@makecaption{\fnum@figure\,---\,continued}{}\par
  \endgroup}
\newcounter{figure}
\def\thefigure{\@arabic\c@figure}
\def\fps@figure{tbp}
\def\ftype@figure{1}
\def\ext@figure{lof}
\def\fnum@figure{\figurename~\thefigure}
\def\figure{%
 \let\@makecaption\@makefigurecaption
  \let\contcaption\contfigurecaption \@float{figure}}
\let\endfigure\end@float
\@namedef{figure*}{%
 \let\@makecaption\@makefigurecaption
  \let\contcaption\contfigurecaption \@dblfloat{figure}}
\@namedef{endfigure*}{\end@dblfloat}
\newcounter{table}
\def\thetable{\@arabic\c@table}
\def\fps@table{tbp}
\def\ftype@table{2}
\def\ext@table{lot}
\def\fnum@table{\tablename~\thetable}
\let\old@floatboxreset\@floatboxreset
\def\table{%
\let\@makecaption\@maketablecaption
\def\@floatboxreset{%
 \old@floatboxreset
 \@tablesize
}%
  \let\footnoterule\relax
  \let\contcaption\conttablecaption \@float{table}}
\let\endtable\end@float
\@namedef{table*}{%
\let\@makecaption\@maketablecaption
\def\@floatboxreset{%
 \old@floatboxreset
 \@tablesize
}%
  \let\footnoterule\relax
  \let\contcaption\conttablecaption \@dblfloat{table}}
\@namedef{endtable*}{\end@dblfloat}
\newtoks\t@glob@notes             % List of all notes
\newtoks\t@loc@notes              % List of notes for one element
\newcount\note@cnt                % Number of notes per element
\newdimen\sv@mathsurround         % Dimen register to save \mathsurround
\newcount\sv@hyphenpenalty        % Count register to save \hyphenpenalty
\newcount\prev@elem \prev@elem=0  % Variables to keep track of
\newcount\cur@elem  \cur@elem=0   % types of elements that are processed
\newif\if@hasabstract             % If abstract / keywords: do not omit rules
\newif\if@haskeywords             % If abstract / keywords: do not omit rules
\newbox\fm@box                    % Box for collected front matter
\newdimen\fm@size                 % Total height of \fm@box
\newbox\t@abstract                % Box for abstract
\newbox\t@keyword                 % Box for keyword abstract
 \let\report@elt\@gobble
\def\add@tok#1#2{\global#1\expandafter{\the#1#2}}
\def\add@xtok#1#2{\begingroup
  \no@harm
  \xdef\@act{\global\noexpand#1{\the#1#2}}\@act
\endgroup}
\def\beg@elem{\global\t@loc@notes={}\global\note@cnt\z@}
\def\@xnamedef#1{\expandafter\xdef\csname #1\endcsname}
\def\no@harm{%
  \let\\=\relax  \let\rm\relax
  \let\ss=\relax \let\ae=\relax \let\oe=\relax
  \let\AE=\relax \let\OE=\relax
  \let\o=\relax  \let\O=\relax
  \let\i=\relax  \let\j=\relax
  \let\aa=\relax \let\AA=\relax
  \let\l=\relax  \let\L=\relax
  \let\d=\relax  \let\b=\relax \let\c=\relax
  \let\bar=\relax
  \def\protect{\noexpand\protect\noexpand}}


%\def\theaddress{\alph{address}}

\newcounter{firstpage}
\newcounter{lastpage}
\let\ESpagenumber\arabic
\def\firstpage#1{\def\@tempa{#1}\ifx\@tempa\@empty\else
  \setcounter{firstpage}{#1}%
  \global\c@page=#1 \ignorespaces\fi}
\setcounter{firstpage}{1}
\let\realpageref\pageref
\setcounter{lastpage}{0}
\def\lastpage#1{\def\@tempa{#1}\ifx\@tempa\@empty\else
  \setcounter{lastpage}{#1}\ignorespaces\fi
  }
\AtEndDocument{%
   \clearpage
   \addtocounter{page}{-1}%
   \immediate\write\@auxout{%
   \string\global\string\c@lastpage=\the\c@page}%
   \addtocounter{page}{1}%
}


\def\today{\number\day\space\ifcase\month\or
  January\or February\or March\or April\or May\or June\or
  July\or August\or September\or October\or November\or December\fi
  \space\number\year}


\pagenumbering{arabic}                      % Arabic page numbers
\let\baselinestretch\@blstr

\pagestyle{myheadings}

\lineskip 1pt            % \lineskip is 1pt for all font sizes.
\normallineskip 1pt
\def\baselinestretch{1.4}
\baselineskip 12bp
\topskip 12bp


%\evensidemargin -.5truecm
%\oddsidemargin  -.5truecm
\footskip 0pt
%\textheight 24.3cm%
%\textwidth 17cm

\setlength\textheight\paperheight
\addtolength\textheight{-2in}
\addtolength\textheight{-\footskip}
\addtolength\textheight{-\topmargin}
\addtolength\textheight{-\headheight}
\addtolength\textheight{-\headsep}

\setlength\textwidth\paperwidth
\addtolength\textwidth{-2in}
\addtolength\textwidth{-\evensidemargin}

\endinput

%%
%% End of cesj.cls 
